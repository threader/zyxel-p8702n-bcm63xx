/*
 * ++Copyright++ 1985, 1993
 * -
 * Copyright (c) 1985, 1993
 *    The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by the University of
 *      California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * -
 * Portions Copyright (c) 1993 by Digital Equipment Corporation.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies, and that
 * the name of Digital Equipment Corporation not be used in advertising or
 * publicity pertaining to distribution of the document or software without
 * specific, written prior permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND DIGITAL EQUIPMENT CORP. DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS.   IN NO EVENT SHALL DIGITAL EQUIPMENT
 * CORPORATION BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 * -
 * --Copyright--
 */

#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/nameser.h>

#include <stdio.h>
#include <netdb.h>
#include <resolv.h>

#include <errno.h>
#include <string.h>
#include <ctype.h>


static const char	digits[] = "0123456789";


/*
 *	Thinking in noninternationalized USASCII (per the DNS spec),
 *	convert this character to lower case if it's upper case.
 */
static int
mklower(int ch) {
	if (ch >= 0x41 && ch <= 0x5A)
		return (ch + 0x20);
	return (ch);
}


/*
 * dn_find(domain, msg, dnptrs, lastdnptr)
 *	Search for the counted-label name in an array of compressed names.
 * return:
 *	offset from msg if found, or -1.
 * notes:
 *	dnptrs is the pointer to the first name on the list,
 *	not the pointer to the start of the message.
 */
static int
dn_find(const u_char *domain, const u_char *msg,
	const u_char * const *dnptrs,
	const u_char * const *lastdnptr)
{
	const u_char *dn, *cp, *sp;
	const u_char * const *cpp;
	u_int n;

	for (cpp = dnptrs; cpp < lastdnptr; cpp++) {
		dn = domain;
		sp = cp = *cpp;
		while ((n = *cp++) != 0) {
			/*
			 * check for indirection
			 */
			switch (n & NS_CMPRSFLGS) {
			case 0:			/* normal case, n == len */
				if (n != *dn++)
					goto next;
				for ((void)NULL; n > 0; n--)
					if (mklower(*dn++) != mklower(*cp++))
						goto next;
				/* Is next root for both ? */
				if (*dn == '\0' && *cp == '\0')
					return (sp - msg);
				if (*dn)
					continue;
				goto next;

			case NS_CMPRSFLGS:	/* indirection */
				cp = msg + (((n & 0x3f) << 8) | *cp);
				break;

			default:	/* illegal type */
				errno = EMSGSIZE;
				return (-1);
			}
		}
 next: ;
	}
	errno = ENOENT;
	return (-1);
}

/*
 * ns_name_pack(src, dst, dstsiz, dnptrs, lastdnptr)
 *	Pack domain name 'domain' into 'comp_dn'.
 * return:
 *	Size of the compressed name, or -1.
 * notes:
 *	'dnptrs' is an array of pointers to previous compressed names.
 *	dnptrs[0] is a pointer to the beginning of the message. The array
 *	ends with NULL.
 *	'lastdnptr' is a pointer to the end of the array pointed to
 *	by 'dnptrs'.
 * Side effects:
 *	The list of pointers in dnptrs is updated for labels inserted into
 *	the message as we compress the name.  If 'dnptr' is NULL, we don't
 *	try to compress names. If 'lastdnptr' is NULL, we don't update the
 *	list.
 */

int
ns_name_pack(const u_char *src, u_char *dst, int dstsiz,
	     const u_char **dnptrs, const u_char **lastdnptr)
{
	u_char *dstp;
	const u_char **cpp, **lpp, *eob, *msg;
	const u_char *srcp;
	unsigned n;
	int l;

	srcp = src;
	dstp = dst;
	eob = dstp + dstsiz;
	lpp = cpp = NULL;
	if (dnptrs != NULL) {
		if ((msg = *dnptrs++) != NULL) {
			for (cpp = dnptrs; *cpp != NULL; cpp++)
				(void)NULL;
			lpp = cpp;	/* end of list to search */
		}
	} else
		msg = NULL;

	/* make sure the domain we are about to add is legal */
	l = 0;
	do {
		n = *srcp;
		if ((n & NS_CMPRSFLGS) != 0) {
			errno = EMSGSIZE;
			return (-1);
		}
		l += n + 1;
		if (l > MAXCDNAME) {
			errno = EMSGSIZE;
			return (-1);
		}
		srcp += n + 1;
	} while (n != 0);

	/* from here on we need to reset compression pointer array on error */
	srcp = src;
	do {
		/* Look to see if we can use pointers. */
		n = *srcp;
		if (n != 0 && msg != NULL) {
			l = dn_find(srcp, msg, (const u_char * const *)dnptrs,
				    (const u_char * const *)lpp);
			if (l >= 0) {
				if (dstp + 1 >= eob) {
					goto cleanup;
				}
				*dstp++ = (l >> 8) | NS_CMPRSFLGS;
				*dstp++ = l % 256;
				return (dstp - dst);
			}
			/* Not found, save it. */
			if (lastdnptr != NULL && cpp < lastdnptr - 1 &&
			    (dstp - msg) < 0x4000) {
				*cpp++ = dstp;
				*cpp = NULL;
			}
		}
		/* copy label to buffer */
		if (n & NS_CMPRSFLGS) {		/* Should not happen. */
			goto cleanup;
		}
		if (dstp + 1 + n >= eob) {
			goto cleanup;
		}
		memcpy(dstp, srcp, n + 1);
		srcp += n + 1;
		dstp += n + 1;
	} while (n != 0);

	if (dstp > eob) {
cleanup:
		if (msg != NULL)
			*lpp = NULL;
		errno = EMSGSIZE;
		return (-1);
	} 
	return (dstp - dst);
}

/*
 * ns_name_pton(src, dst, dstsiz)
 *	Convert a ascii string into an encoded domain name as per RFC1035.
 * return:
 *	-1 if it fails
 *	1 if string was fully qualified
 *	0 is string was not fully qualified
 * notes:
 *	Enforces label and domain length limits.
 */

int
ns_name_pton(const char *src, u_char *dst, size_t dstsiz) {
	u_char *label, *bp, *eom;
	int c, n, escaped;
	char *cp;

	escaped = 0;
	bp = dst;
	eom = dst + dstsiz;
	label = bp++;

	while ((c = *src++) != 0) {
		if (escaped) {
			if ((cp = strchr(digits, c)) != NULL) {
				n = (cp - digits) * 100;
				if ((c = *src++) == 0 ||
				    (cp = strchr(digits, c)) == NULL) {
					errno = EMSGSIZE;
					return (-1);
				}
				n += (cp - digits) * 10;
				if ((c = *src++) == 0 ||
				    (cp = strchr(digits, c)) == NULL) {
					errno = EMSGSIZE;
					return (-1);
				}
				n += (cp - digits);
				if (n > 255) {
					errno = EMSGSIZE;
					return (-1);
				}
				c = n;
			}
			escaped = 0;
		} else if (c == '\\') {
			escaped = 1;
			continue;
		} else if (c == '.') {
			c = (bp - label - 1);
			if ((c & NS_CMPRSFLGS) != 0) {	/* Label too big. */
				errno = EMSGSIZE;
				return (-1);
			}
			if (label >= eom) {
				errno = EMSGSIZE;
				return (-1);
			}
			*label = c;
			/* Fully qualified ? */
			if (*src == '\0') {
				if (c != 0) {
					if (bp >= eom) {
						errno = EMSGSIZE;
						return (-1);
					}
					*bp++ = '\0';
				}
				if ((bp - dst) > MAXCDNAME) {
					errno = EMSGSIZE;
					return (-1);
				}
				return (1);
			}
			if (c == 0 || *src == '.') {
				errno = EMSGSIZE;
				return (-1);
			}
			label = bp++;
			continue;
		}
		if (bp >= eom) {
			errno = EMSGSIZE;
			return (-1);
		}
		*bp++ = (u_char)c;
	}
	c = (bp - label - 1);
	if ((c & NS_CMPRSFLGS) != 0) {		/* Label too big. */
		errno = EMSGSIZE;
		return (-1);
	}
	if (label >= eom) {
		errno = EMSGSIZE;
		return (-1);
	}
	*label = c;
	if (c != 0) {
		if (bp >= eom) {
			errno = EMSGSIZE;
			return (-1);
		}
		*bp++ = 0;
	}
	if ((bp - dst) > MAXCDNAME) {	/* src too big */
		errno = EMSGSIZE;
		return (-1);
	}
	return (0);
}



/*
 * ns_name_compress(src, dst, dstsiz, dnptrs, lastdnptr)
 *	Compress a domain name into wire format, using compression pointers.
 * return:
 *	Number of bytes consumed in `dst' or -1 (with errno set).
 * notes:
 *	'dnptrs' is an array of pointers to previous compressed names.
 *	dnptrs[0] is a pointer to the beginning of the message.
 *	The list ends with NULL.  'lastdnptr' is a pointer to the end of the
 *	array pointed to by 'dnptrs'. Side effect is to update the list of
 *	pointers for labels inserted into the message as we compress the name.
 *	If 'dnptr' is NULL, we don't try to compress names. If 'lastdnptr'
 *	is NULL, we don't update the list.
 */
int
ns_name_compress(const char *src, u_char *dst, size_t dstsiz,
		 const u_char **dnptrs, const u_char **lastdnptr)
{
	u_char tmp[NS_MAXCDNAME];

	if (ns_name_pton(src, tmp, sizeof tmp) == -1)
		return (-1);
	return (ns_name_pack(tmp, dst, dstsiz, dnptrs, lastdnptr));
}



/*
 * Pack domain name 'exp_dn' in presentation form into 'comp_dn'.
 * Return the size of the compressed name or -1.
 * 'length' is the size of the array pointed to by 'comp_dn'.
 */
int
dn_comp(const char *src, u_char *dst, int dstsiz,
	u_char **dnptrs, u_char **lastdnptr)
{
	return (ns_name_compress(src, dst, (size_t)dstsiz,
				 (const u_char **)dnptrs,
				 (const u_char **)lastdnptr));
}



void putUShort (obuf, val)
	unsigned char *obuf;
	u_int32_t val;
{
	u_int16_t tmp = htons (val);
	memcpy (obuf, &tmp, sizeof tmp);
}

void putULong (obuf, val)
	unsigned char *obuf;
	u_int32_t val;
{
	u_int32_t tmp = htonl (val);
	memcpy (obuf, &tmp, sizeof tmp);
}    


/*
 * Form all types of queries.
 * Returns the size of the result or -1.
 */
int
res_mkquery(op, dname, class, type, data, datalen, newrr_in, buf, buflen)
        int op;                 /* opcode of query */
        const char *dname;      /* domain name */
        int class, type;        /* class and type of query */
        const u_char *data;     /* resource record data */
        int datalen;            /* length of data */
        const u_char *newrr_in; /* new rr for modify or append */
        u_char *buf;            /* buffer to put query */
        int buflen;             /* size of buffer */
{
        register HEADER *hp;
        register u_char *cp;
        register int n;

        u_char *dnptrs[20], **dpp, **lastdnptr;

        if ((_res.options & RES_INIT) == 0 && res_init() == -1) {
                h_errno = NETDB_INTERNAL;
                return (-1);
        }
        /*
         * Initialize header fields.
         */
        if ((buf == NULL) || (buflen < HFIXEDSZ))
                return (-1);
        bzero(buf, HFIXEDSZ);
        hp = (HEADER *) buf;
        hp->id = htons(++_res.id);
        hp->opcode = op;
        hp->rd = (_res.options & RES_RECURSE) != 0;
        hp->rcode = NOERROR;
        cp = buf + HFIXEDSZ;
        buflen -= HFIXEDSZ;
        dpp = dnptrs;
        *dpp++ = buf;
        *dpp++ = NULL;
        lastdnptr = dnptrs + sizeof dnptrs / sizeof dnptrs[0];
        /*
         * perform opcode specific processing
         */
        switch (op) {
        case QUERY:     /*FALLTHROUGH*/
        case NS_NOTIFY_OP:
                if ((buflen -= QFIXEDSZ) < 0)
                        return (-1);
                if ((n = dn_comp(dname, cp, buflen, dnptrs, lastdnptr)) < 0)
                        return (-1);
                cp += n;
                buflen -= n;
                putUShort(cp, type);
                cp += INT16SZ;
                putUShort(cp, class);
                cp += INT16SZ;
                hp->qdcount = htons(1);
                if (op == QUERY || data == NULL)
                        break;
                /*
                 * Make an additional record for completion domain.
                 */
                buflen -= RRFIXEDSZ;
                n = dn_comp((char *)data, cp, buflen, dnptrs, lastdnptr);
                if (n < 0)
                        return (-1);
                cp += n;
                buflen -= n;
                putUShort(cp, T_NULL);
                cp += INT16SZ;
                putUShort(cp, class);
                cp += INT16SZ;
                putULong(cp, 0);
                cp += INT32SZ;
                putUShort(cp, 0);
                cp += INT16SZ;
                hp->arcount = htons(1);
                break;

        case IQUERY:
                /*
                 * Initialize answer section
                 */
                if (buflen < 1 + RRFIXEDSZ + datalen)
                        return (-1);
                *cp++ = '\0';   /* no domain name */
                putUShort(cp, type);
                cp += INT16SZ;
                putUShort(cp, class);
                cp += INT16SZ;
                putULong(cp, 0);
                cp += INT32SZ;
                putUShort(cp, datalen);
                cp += INT16SZ;
                if (datalen) {
                        memcpy(data, cp, datalen);
                        cp += datalen;
                }
                hp->ancount = htons(1);
                break;
        default:
                return (-1);
        }
        return cp - ((u_char *)buf);
}

