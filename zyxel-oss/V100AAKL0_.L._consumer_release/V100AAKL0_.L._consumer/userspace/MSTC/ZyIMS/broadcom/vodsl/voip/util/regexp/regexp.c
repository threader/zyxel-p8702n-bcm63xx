/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16251 Laguna Canyon Road
*             Irvine, California  92618
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: regexp.c
*    Creation Date: 27 June 1999 (v0.00)
*    VSS Info:
*        $Revision: 1 $
*        $Date: 10/03/01 6:23p $
*
****************************************************************************
*    Description:
*
*      This file contains the regular expression functionality used in SGCP/MGCP
*
****************************************************************************/

/*
 * regcomp and regexec
 *
 *  Copyright (c) 1986 by University of Toronto.
 *  Written by Henry Spencer.  Not derived from licensed software.
 *
 *  Permission is granted to anyone to use this software for any
 *  purpose on any computer system, and to redistribute it freely,
 *  subject to the following restrictions:
 *
 *  1. The author is not responsible for the consequences of use of
 *      this software, no matter how awful, even if they arise
 *      from defects in it.
 *
 *  2. The origin of this software must not be misrepresented, either
 *      by explicit claim or by omission.
 *
 *  3. Altered versions must be plainly marked as such, and must not
 *      be misrepresented as being the original software.
 *
 * Beware that some of this code is subtly aware of the way operator
 * precedence is structured in regular expressions.  Serious changes in
 * regular-expression syntax might require a total rethink.
 *
 * ALTERED VERSION: June 1999
 * This code has been seriously munged to do SGCP/MGCP DigitMap expression
 * matching. It has smaller, less capable syntax and different metachars
 * than standard UNIX regular expressions.
 */
#include <stdio.h>
#include <stdlib.h>                 /* MG */
#include <string.h>                 /* MG */
#include <ctype.h>                  /* MG */
#include "regexp.h"
#ifndef LINUX
char *strchr();
#endif

/*
 * The "internal use only" fields in regexp.h are present to pass info from
 * compile to execute that permits the execute phase to run lots faster on
 * simple cases.  They are:
 *
 * regstart char that must begin a match; '\0' if none obvious
 * reganch  is the match anchored (at beginning-of-line only)?
 * regmust  string (pointer into program) that match must include, or NULL
 * regmlen  length of regmust string
 *
 * Regstart and reganch permit very fast decisions on suitable starting points
 * for a match, cutting down the work a lot.  Regmust permits fast rejection
 * of lines that cannot possibly match.  The regmust tests are costly enough
 * that regcomp() supplies a regmust only if the r.e. contains something
 * potentially expensive (at present, the only such thing detected is * or +
 * at the start of the r.e., which can involve a lot of backup).  Regmlen is
 * supplied because the test in regexec() needs it and regcomp() is
 * computing it anyway.
 */

/*
 * Structure for regexp "program".  This is essentially a linear encoding
 * of a nondeterministic finite-state machine (aka syntax charts or
 * "railroad normal form" in parsing technology).  Each node is an opcode
 * plus a "next" pointer, possibly plus an operand.  "Next" pointers of
 * all nodes except BRANCH implement concatenation; a "next" pointer with
 * a BRANCH on both ends of it is connecting two alternatives.  (Here we
 * have one of the subtle syntax dependencies:  an individual BRANCH (as
 * opposed to a collection of them) is never concatenated with anything
 * because of operator precedence.)  The operand of some types of node is
 * a literal string; for others, it is a node leading into a sub-FSM.  In
 * particular, the operand of a BRANCH node is the first node of the branch.
 * (NB this is *not* a tree structure:  the tail of the branch connects
 * to the thing following the set of BRANCHes.)  The opcodes are:
 */

/* definition   number  opnd?   meaning */
#undef EOL
#define  END   0               /* no End of program. */
/* MG #define    BOL 1 */      /* no   Match "" at beginning of line. */
/* MG #define    EOL 2 */      /* no   Match "" at end of line. */
#define  ANY   3               /* no Match any one character. */
#define  ANYOF 4               /* str    Match any character in this string. */
/* MG #define    ANYBUT  5 */  /* str  Match any character not in this string. */
#define  BRANCH   6            /* node   Match this alternative, or the next... */
#define  BACK  7               /* no Match "", "next" ptr points backward. */
#define  EXACTLY  8            /* str    Match this string. */
#define  NOTHING  9            /* no Match empty string. */
#define  STAR  10              /* node   Match this (simple) thing 0 or more times. */
/* MG #define    PLUS    11 */ /* node Match this (simple) thing 1 or more times. */
#define  OPEN  20              /* no Mark this point in input as start of #n. */
                               /*  OPEN+1 is number 1, etc. */
#define  CLOSE 30              /* no Analogous to OPEN. */

/*
 * Opcode notes:
 *
 * BRANCH   The set of branches constituting a single choice are hooked
 *      together with their "next" pointers, since precedence prevents
 *      anything being concatenated to any individual branch.  The
 *      "next" pointer of the last BRANCH in a choice points to the
 *      thing following the whole choice.  This is also where the
 *      final "next" pointer of each individual branch points; each
 *      branch starts with the operand node of a BRANCH node.
 *
 * BACK     Normal "next" pointers all implicitly point forward; BACK
 *      exists to make loop structures possible.
 *
 * STAR,PLUS    '?', and complex '*' and '+', are implemented as circular
 *      BRANCH structures using BACK.  Simple cases (one character
 *      per match) are implemented with STAR and PLUS for speed
 *      and to minimize recursive plunges.
 *
 * OPEN,CLOSE   ...are numbered at compile time.
 */

/*
 * A node is one char of opcode followed by two chars of "next" pointer.
 * "Next" pointers are stored as two 8-bit pieces, high order first.  The
 * value is a positive offset from the opcode of the node containing it.
 * An operand, if any, simply follows the node.  (Note that much of the
 * code generation knows about this implicit relationship.)
 *
 * Using two bytes for the "next" pointer is vast overkill for most things,
 * but allows patterns to get big without disasters.
 */
#define  OP(p)       (*(p))
#define  NEXT(p)     (((*((p)+1)&0377)<<8) + (*((p)+2)&0377))
#define  OPERAND(p)  ((p) + 3)

/*
 * See regmagic.h for one further detail of program structure.
 */
#define MAXEXPR 2000
#define NSUBEXP 2
typedef struct regexp
{
   char *startp[NSUBEXP];
   char *endp[NSUBEXP];
   char *regmust;                /* Internal use only. */
   int regmlen;                  /* Internal use only. */
   char regstart;                /* Internal use only. */
   char program[1];              /* Unwarranted chumminess with compiler. */
}
regexp;



/*
 * Utility definitions.
 */
#ifndef CHARBITS
#define  UINT8AT(p)  ((int)*(unsigned char *)(p))
#else
#define  UINT8AT(p)  ((int)*(p)&CHARBITS)
#endif

#define regerror(m)

#define FAIL(m)   { regerror(m); return(NULL); }
#define ISMULT(c) ((c) == '.')            /* MG '*' || (c) == '+' || (c) == '?') */
#define META      "[|X.()"                /* MG "^$[()|?+*\\" */

/*
 * Flags to be passed up and down.
 */
#define  HASWIDTH 01             /* Known never to match null string. */
#define  SIMPLE   02             /* Simple enough to be STAR/PLUS operand. */
#define  SPSTART  04             /* Starts with * or +. */
#define  WORST    0              /* Worst case. */

/*
 * Global work variables for regcomp().
 */
static char *regparse;           /* Input-scan pointer. */
static int regnpar;              /* () count. */
static char *regcode;            /* Code-emit pointer; &regdummy = don't. */
static long regsize;             /* Code size. */

/*
 * The first byte of the regexp internal "program" is actually this magic
 * number; the start node begins in the second byte.
 */
#define  MAGIC 0234


/*
 * Forward declarations for regcomp()'s friends.
 */
static char *reg(int paren, int *flags);
static char *regbranch(int *flags);
static char *regpiece(int *flags);
static char *regatom(int *flags);
static char *regnode(char op);
static char *regnext(char *p);
static void regc(char b);
static void reginsert(char op, char *opnd);
static void regtail(char *p, char *val);
static void regoptail(char *p, char *val);

/*
   - regcomp - compile a regular expression into internal code
   *
 */
void *
regcomp(void *regbuf, char *exp)
{
   regexp *r;
   char *scan;
/* char *longest;
   int len;
*/
   int flags;

   if (exp == NULL)
      FAIL("NULL argument");

   regsize = 0;
   /* Allocate space. */
   r = (regexp *) regbuf;

   regparse = exp;
   regnpar = 1;
   regcode = r->program;
   regc((char)MAGIC);
   if (reg(0, &flags) == NULL)
      return (NULL);

   /* Dig out information for optimizations. */
   r->regstart = '\0';              /* Worst-case defaults. */
   r->regmust = NULL;
   r->regmlen = 0;
   scan = r->program + 1;           /* First BRANCH. */
   if (OP(regnext(scan)) == END)
   {                          /* Only one top-level choice. */
      scan = OPERAND(scan);

      /* Starting-point info. */
      if (OP(scan) == EXACTLY)
         r->regstart = *OPERAND(scan);
      /*
       * If there's something expensive in the r.e., find the
       * longest literal string that must appear and make it the
       * regmust.  Resolve ties in favor of later strings, since
       * the regstart check works with the beginning of the r.e.
       * and avoiding duplication strengthens checking.  Not a
       * strong reason, but sufficient in the absence of others.
       */

      /*
       * We don't need optimization (that will potentially screw up
       * the code
       */
/*
      if (flags & SPSTART)
      {
         longest = NULL;
         len = 0;
         for (; scan != NULL; scan = regnext(scan))
            if (OP(scan) == EXACTLY && ((int)strlen(OPERAND(scan))) >= len)
            {
               longest = OPERAND(scan);
               len = strlen(OPERAND(scan));
            }
         r->regmust = longest;
         r->regmlen = len;
      }
*/
   }

   return (r);
}

/*
   - reg - regular expression, i.e. main body or parenthesized thing
   *
   * Caller must absorb opening parenthesis.
   *
   * Combining parenthesis handling with the base level of regular expression
   * is a trifle forced, but the need to tie the tails of the branches to what
   * follows makes it hard to avoid.
 */
static char *
reg(int paren, int *flagp)
{
   char *ret;
   char *br;
   char *ender;
   int parno = 0;
   int flags;

   *flagp = HASWIDTH;               /* Tentatively. */

   /* Make an OPEN node, if parenthesized. */
   if (paren)
   {
      if (regnpar >= NSUBEXP)
         FAIL("too many ()");
      parno = regnpar;
      regnpar++;
      ret = regnode((char)(OPEN + parno));
   }
   else
      ret = NULL;

   /* Pick up the branches, linking them together. */
   br = regbranch(&flags);
   if (br == NULL)
      return (NULL);
   if (ret != NULL)
      regtail(ret, br);          /* OPEN -> first. */
   else
      ret = br;
   if (!(flags & HASWIDTH))
      *flagp &= ~HASWIDTH;
   *flagp |= flags & SPSTART;
   while (*regparse == '|')
   {
      regparse++;
      br = regbranch(&flags);
      if (br == NULL)
         return (NULL);
      regtail(ret, br);          /* BRANCH -> BRANCH. */
      if (!(flags & HASWIDTH))
         *flagp &= ~HASWIDTH;
      *flagp |= flags & SPSTART;
   }

   /* Make a closing node, and hook it on the end. */
   ender = regnode( (char)((paren) ? (char)(CLOSE + parno) : (char)END) );
   regtail(ret, ender);

   /* Hook the tails of the branches to the closing node. */
   for (br = ret; br != NULL; br = regnext(br))
      regoptail(br, ender);

   /* Check for proper termination. */
   if (paren && *regparse++ != ')')
   {
      FAIL("unmatched ()");
   }
   else if (!paren && *regparse != '\0')
   {
      if (*regparse == ')')
      {
         FAIL("unmatched ()");
      }
      else
         FAIL("junk on end");    /* "Can't happen". */
      /* NOTREACHED */
   }

   return (ret);
}

/*
   - regbranch - one alternative of an | operator
   *
   * Implements the concatenation operator.
 */
static char *
regbranch(int *flagp)
{
   char *ret;
   char *chain;
   char *latest;
   int flags;

   *flagp = WORST;                  /* Tentatively. */

   ret = regnode(BRANCH);
   chain = NULL;
   while (*regparse != '\0' && *regparse != '|' && *regparse != ')')
   {
      latest = regpiece(&flags);
      if (latest == NULL)
         return (NULL);
      *flagp |= flags & HASWIDTH;
      if (chain == NULL)            /* First piece. */
         *flagp |= flags & SPSTART;
      else
         regtail(chain, latest);
      chain = latest;
   }
   if (chain == NULL)               /* Loop ran zero times. */
      (void)regnode(NOTHING);

   return (ret);
}

/*
   - regpiece - something followed by possible [*+?]
   *
   * Note that the branching code sequences used for ? and the general cases
   * of * and + are somewhat optimized:  they use the same NOTHING node as
   * both the endmarker for their branch list and the body of the last branch.
   * It might seem that this node could be dispensed with entirely, but the
   * endmarker role is not redundant.
 */
static char *
regpiece(int *flagp)
{
   char *ret;
   char op;
   int flags;

   ret = regatom(&flags);
   if (ret == NULL)
      return (NULL);

   op = *regparse;
   if (!ISMULT(op))
   {
      *flagp = flags;
      return (ret);
   }

   *flagp = (WORST | SPSTART);

   if (op == '.' && (flags & SIMPLE))
      reginsert(STAR, ret);
   else if (op == '.')
   {
      /* Emit x* as (x&|), where & means "self". */
      reginsert(BRANCH, ret);       /* Either x */
      regoptail(ret, regnode(BACK));   /* and loop */
      regoptail(ret, ret);       /* back */
      regtail(ret, regnode(BRANCH));   /* or */
      regtail(ret, regnode(NOTHING));  /* null. */
   }
   regparse++;
   if (ISMULT(*regparse))
      FAIL("nested .");

   return (ret);
}

/*
   - regatom - the lowest level
   *
   * Optimization:  gobbles an entire sequence of ordinary characters so that
   * it can turn them into a single node, which is smaller to store and
   * faster to run.  Backslashed characters are exceptions, each becoming a
   * separate node; the code is simpler that way and it's not worth fixing.
 */
static char *
regatom(int *flagp)
{
   char *ret;
   int flags;

   *flagp = WORST;                  /* Tentatively. */

   switch (*regparse++)
   {
   case 'X':
      ret = regnode(ANY);
      *flagp |= HASWIDTH | SIMPLE;
      break;
   case '[':
      {
         register int clss;
         register int classend;

         ret = regnode(ANYOF);
         if (*regparse == ']' || *regparse == '-')
            regc(*regparse++);
         while (*regparse != '\0' && *regparse != ']')
         {
            if (*regparse == '-')
            {
               regparse++;
               if (*regparse == ']' || *regparse == '\0')
                  regc('-');
               else
               {
                  clss = UINT8AT(regparse - 2) + 1;
                  classend = UINT8AT(regparse);
                  if (clss > classend + 1)
                     FAIL("invalid [] range");
                  for (; clss <= classend; clss++)
                     regc((char)clss);
                  regparse++;
               }
            }
            else
               regc(*regparse++);
         }
         regc('\0');
         if (*regparse != ']')
            FAIL("unmatched []");
         regparse++;
         *flagp |= HASWIDTH | SIMPLE;
      }
      break;
   case '(':
      ret = reg(1, &flags);
      if (ret == NULL)
         return (NULL);
      *flagp |= flags & (HASWIDTH | SPSTART);
      break;
   case '\0':
   case '|':
   case ')':
      FAIL("internal urp");         /* Supposed to be caught earlier. */
      /* NOTREACHED */
      break;
   case '.':
      FAIL(". follows nothing");
      /* NOTREACHED */
      break;
   default:
      {
         register int len;
         register char ender;

         regparse--;
         len = strcspn(regparse, META);
         if (len <= 0)
            FAIL("internal disaster");
         ender = *(regparse + len);
         if (len > 1 && ISMULT(ender))
            len--;               /* Back off clear of ?+* operand. */
         *flagp |= HASWIDTH;
         if (len == 1)
            *flagp |= SIMPLE;
         ret = regnode(EXACTLY);
         while (len > 0)
         {
            regc(*regparse++);
            len--;
         }
         regc('\0');
      }
      break;
   }

   return (ret);
}

/*
   - regnode - emit a node
 */
static char *                    /* Location. */
regnode(char op)
{
   char *ret;
   char *ptr;

   ret = regcode;
   regsize += 3;
   if (regsize > MAXEXPR)
      return NULL;

   ptr = ret;
   *ptr++ = op;
   *ptr++ = '\0';                /* Null "next" pointer. */
   *ptr++ = '\0';
   regcode = ptr;

   return (ret);
}

/*
   - regc - emit (if appropriate) a byte of code
 */
static void
regc(char b)
{
   if (++regsize > MAXEXPR)
      return;
   *regcode++ = b;
}

/*
   - reginsert - insert an operator in front of already-emitted operand
   *
   * Means relocating the operand.
 */
static void
reginsert(char op, char *opnd)
{
   char *src;
   char *dst;
   char *place;

   regsize += 3;

   src = regcode;
   regcode += 3;
   dst = regcode;
   while (src > opnd)
      *--dst = *--src;

   place = opnd;                 /* Op node, where operand used to be. */
   *place = op;
   /* *place++ = '\0'; */   /* get rid of warnings */
   /* *place++ = '\0'; */
}

/*
   - regtail - set the next-pointer at the end of a node chain
 */
static void
regtail(char *p, char *val)
{
   char *scan;
   char *temp;
   int offset;

   /* Find last node. */
   scan = p;
   for (;;)
   {
      temp = regnext(scan);
      if (temp == NULL)
         break;
      scan = temp;
   }

   if (OP(scan) == BACK)
      offset = scan - val;
   else
      offset = val - scan;
   *(scan + 1) = (char)((offset >> 8) & 0377);
   *(scan + 2) = (char)(offset & 0377);
}

/*
   - regoptail - regtail on operand of first argument; nop if operandless
 */
static void
regoptail(char *p, char *val)
{
   /* "Operandless" and "op != BRANCH" are synonymous in practice. */
   if (p == NULL || OP(p) != BRANCH)
      return;
   regtail(OPERAND(p), val);
}

/*
 * regexec and friends
 */

/*
 * Global work variables for regexec().
 */
static char *reginput;              /* String-input pointer. */
static char *regbol;             /* Beginning of input, for ^ check. */
static char **regstartp;            /* Pointer to startp array. */
static char **regendp;              /* Ditto for endp. */

/*
 * Forwards.
 */
static int regtry(regexp * prog, char *string);
static int regmatch(char *prog);
static int regrepeat(char *p);

#ifdef DEBUG
int regnarrate = 0;
void regdump();
static char *regprop();

#endif
int endinp;

/*
   - regexec - match a regexp against a string
   0 = cant match, -1 = partial match, 1 = match
 */
int
regexec(void *rprog, char *string)
{
   regexp *prog = (regexp *) rprog;
   char *s;

   endinp = 0;

   /* Be paranoid... */
   if (prog == NULL || string == NULL)
   {
      regerror("NULL parameter");
      return (0);
   }

   /* Check validity of program. */
   if (UINT8AT(prog->program) != MAGIC)
   {
      regerror("corrupted program");
      return (0);
   }

   /* If there is a "must appear" string, look for it. */
   if (prog->regmust != NULL)
   {
      s = string;
      while ((s = strchr(s, prog->regmust[0])) != NULL)
      {
         if (strncmp(s, prog->regmust, prog->regmlen) == 0)
            break;               /* Found it. */
         s++;
      }
      if (s == NULL)             /* Not present. */
         return (0);
   }

   /* Mark beginning of line for ^ . */
   regbol = string;

   /* Simplest case:  anchored match need be tried only once. */
   if (regtry(prog, string))
      return 1;
   else if (endinp)
      return -1;
   else
      return 0;
}

/*
   - regtry - try match at specific point
 */
static int                       /* 0 failure, 1 success */
regtry(regexp * prog, char *string)
{
   int i;
   char **sp;
   char **ep;

   reginput = string;
   regstartp = prog->startp;
   regendp = prog->endp;

   sp = prog->startp;
   ep = prog->endp;
   for (i = NSUBEXP; i > 0; i--)
   {
      *sp++ = NULL;
      *ep++ = NULL;
   }

   if (regmatch(prog->program + 1))
   {
      return (1);
   }
   else
      return (0);
}

/*
   - regmatch - main matching routine
   *
   * Conceptually the strategy is simple:  check to see whether the current
   * node matches, call self recursively to see whether the rest matches,
   * and then act accordingly.  In practice we make some effort to avoid
   * recursion, in particular by going through "ordinary" nodes (that don't
   * need to know whether the rest of the match failed) by a loop instead of
   * by recursion.
 */
static int                       /* 0 failure, 1 success */
regmatch(prog)
    char *prog;
{
   char *scan;                   /* Current node. */
   char *next;                   /* Next node. */


   scan = prog;
#ifdef DEBUG
   if (scan != NULL && regnarrate)
      fprintf(stderr, "%s(\n", regprop(scan));
#endif
   while (scan != NULL)
   {
#ifdef DEBUG
      if (regnarrate)
         fprintf(stderr, "%s...\n", regprop(scan));
#endif
      next = regnext(scan);

      if (*reginput == '\0')
         endinp = 1;

      switch (OP(scan))
      {
      case ANY:
         if ((*reginput == '\0') || !isdigit(*reginput))
            return (0);
         reginput++;
         break;
      case EXACTLY:
         {
            char *opnd;
            char *s;

            opnd = OPERAND(scan);
            s = reginput;
            while (*s && *opnd && *s == *opnd)
            {
               s++;
               opnd++;
            }
            if (*opnd)
            {
               if (!*s)
                  endinp = 1;
               return (0);
            }
            reginput = s;
         }
         break;

      case ANYOF:
         if (*reginput == '\0' || strchr(OPERAND(scan), *reginput) == NULL)
            return (0);
         reginput++;
         break;
      case NOTHING:
         break;
      case BACK:
         break;
      case OPEN + 1:
      case OPEN + 2:
         {
            register int no;
            register char *save;

            no = OP(scan) - OPEN;
            save = reginput;

            if (regmatch(next))
            {
               /*
                * Don't set startp if some later
                * invocation of the same parentheses
                * already has.
                */
               if (regstartp[no] == NULL)
                  regstartp[no] = save;
               return (1);
            }
            else
               return (0);
         }
         /* NOTREACHED */
         break;
      case CLOSE + 1:
      case CLOSE + 2:
         {
            register int no;
            register char *save;

            no = OP(scan) - CLOSE;
            save = reginput;

            if (regmatch(next))
            {
               /*
                * Don't set endp if some later
                * invocation of the same parentheses
                * already has.
                */
               if (regendp[no] == NULL)
                  regendp[no] = save;
               return (1);
            }
            else
               return (0);
         }
         /* NOTREACHED */
         break;
      case BRANCH:
         {
            register char *save;

            if (OP(next) != BRANCH) /* No choice. */
               next = OPERAND(scan);   /* Avoid recursion. */
            else
            {
               do
               {
                  save = reginput;
                  if (regmatch(OPERAND(scan)))
                     return (1);
                  reginput = save;
                  scan = regnext(scan);
               }
               while (scan != NULL && OP(scan) == BRANCH);
               return (0);
               /* NOTREACHED */
            }
         }
         /* NOTREACHED */
         break;
      case STAR:
         {
            register char nextch;
            register int no;
            register char *save;
            register int min;

            /*
             * Lookahead to avoid useless match attempts
             * when we know what character comes next.
             */
            nextch = '\0';
            if (OP(next) == EXACTLY)
               nextch = *OPERAND(next);

            min = (OP(scan) == STAR) ? 0 : 1;
            save = reginput;
            no = regrepeat(OPERAND(scan));
            while (no >= min)
            {
               /* If it could work, try it. */
               if (nextch == '\0' || *reginput == nextch)
                  if (regmatch(next))
                     return (1);
               /* Couldn't or didn't -- back up. */
               no--;
               reginput = save + no;
            }
            /* STAR is always a partial match for MGCP */
            endinp = 1;  /* MG */

            return (0);
         }
         /* NOTREACHED */
         break;
      case END:
         return (1);             /* Success! */
         /* NOTREACHED */
         break;
      default:
         regerror("memory corruption");
         return (0);
         /* NOTREACHED */
         break;
      }

      scan = next;
   }

   /*
    * We get here only if there's trouble -- normally "case END" is
    * the terminating point.
    */
   regerror("corrupted pointers");
   return (0);
}

/*
   - regrepeat - repeatedly match something simple, report how many
 */
static int
regrepeat(char *p)
{
   int count = 0;
   char *scan;
   char *opnd;

   scan = reginput;
   opnd = OPERAND(p);
   switch (OP(p))
   {
   case ANY:
      count = strlen(scan);
      scan += count;
      break;
   case EXACTLY:
      while (*opnd == *scan)
      {
         count++;
         scan++;
      }
      break;
   case ANYOF:
      while (*scan != '\0' && strchr(opnd, *scan) != NULL)
      {
         count++;
         scan++;
      }
      break;
   default:                   /* Oh dear.  Called inappropriately. */
      regerror("internal foulup");
      count = 0;                 /* Best compromise. */
      break;
   }
   reginput = scan;

   return (count);
}

/*
   - regnext - dig the "next" pointer out of a node
 */
static char *
regnext(char *p)
{
   int offset;

   offset = NEXT(p);
   if (offset == 0)
      return (NULL);

   if (OP(p) == BACK)
      return (p - offset);
   else
      return (p + offset);
}

#ifdef DEBUG

static char *regprop();

/*
   - regdump - dump a regexp onto stdout in vaguely comprehensible form
 */
void
regdump(r)
    regexp *r;
{
   char *s;
   char op = EXACTLY;               /* Arbitrary non-END op. */
   char *next;


   s = r->program + 1;
   while (op != END)
   {                          /* While that wasn't END last time... */
      op = OP(s);
      printf("%2d%s", s - r->program, regprop(s)); /* Where, what. */
      next = regnext(s);
      if (next == NULL)          /* Next ptr. */
         printf("(0)");
      else
         printf("(%d)", (s - r->program) + (next - s));
      s += 3;
      if (op == ANYOF || op == EXACTLY)
      {
         /* Literal string, where present. */
         while (*s != '\0')
         {
            putchar(*s);
            s++;
         }
         s++;
      }
      putchar('\n');
   }

   /* Header fields of interest. */
   if (r->regstart != '\0')
      printf("start `%c' ", r->regstart);
   if (r->regmust != NULL)
      printf("must have \"%s\"", r->regmust);
   printf("\n");
}

/*
   - regprop - printable representation of opcode
 */
static char *
regprop(op)
    char *op;
{
   char *p;
   static char buf[50];

   (void)strcpy(buf, ":");

   switch (OP(op))
   {
   case ANY:
      p = "ANY";
      break;
   case ANYOF:
      p = "ANYOF";
      break;
   case EXACTLY:
      p = "EXACTLY";
      break;
   case BRANCH:
      p = "BRANCH";
      break;
   case NOTHING:
      p = "NOTHING";
      break;
   case BACK:
      p = "BACK";
      break;
   case OPEN + 1:
   case OPEN + 2:
      sprintf(buf + strlen(buf), "OPEN%d", OP(op) - OPEN);
      p = NULL;
      break;
   case CLOSE + 1:
   case CLOSE + 2:
      sprintf(buf + strlen(buf), "CLOSE%d", OP(op) - CLOSE);
      p = NULL;
      break;
   case END:
      p = "END";
      break;
   case STAR:
      p = "STAR";
      break;
   default:
      regerror("corrupted opcode");
      break;
   }
   if (p != NULL)
      (void)strcat(buf, p);
   return (buf);
}
#endif
