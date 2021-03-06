/* Copyright 1998 by the Massachusetts Institute of Technology.
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in
 * advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 * M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 */

static const char rcsid[] = "$Id: ares__get_hostent.c,v 1.2 2006/08/10 13:02:55 umsingh Exp $";

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <netdb.h>
#include "ares.h"
#include "ares_private.h"

int ares__get_hostent(FILE *fp, struct hostent **host)
{
  char *line = NULL, *p, *q, *canonical, **alias;
  int status, linesize, end_at_hostname, naliases;
  struct in_addr addr;
  struct hostent *hostent = NULL;

  while ((status = ares__read_line(fp, &line, &linesize)) == ARES_SUCCESS)
    {
      /* Skip comment lines; terminate line at comment character. */
      if (*line == '#' || !*line)
	continue;
      p = strchr(line, '#');
      if (p)
	*p = 0;

      /* Get the address part. */
      p = line;
      while (*p && !isspace((unsigned char)*p))
	p++;
      if (!*p)
	continue;
      *p = 0;
      addr.s_addr = inet_addr(line);
      if (addr.s_addr == INADDR_NONE)
	continue;

      /* Get the canonical hostname. */
      p++;
      while (isspace((unsigned char)*p))
	p++;
      if (!*p)
	continue;
      q = p;
      while (*q && !isspace((unsigned char)*q))
	q++;
      end_at_hostname = (*q == 0);
      *q = 0;
      canonical = p;

      naliases = 0;
      if (!end_at_hostname)
	{
	  /* Count the aliases. */
	  p = q + 1;
	  while (isspace((unsigned char)*p))
	    p++;
	  while (*p)
	    {
	      while (*p && !isspace((unsigned char)*p))
		p++;
	      while (isspace((unsigned char)*p))
		p++;
	      naliases++;
	    }
	}

      /* Allocate memory for the host structure. */
      hostent = (struct hostent*)malloc(sizeof(struct hostent));
      if (!hostent)
	break;
      hostent->h_aliases = NULL;
      hostent->h_addr_list = NULL;
      hostent->h_name = strdup(canonical);
      if (!hostent->h_name)
	break;
      hostent->h_addr_list = (char**) malloc(2 * sizeof(char *));
      if (!hostent->h_addr_list)
	break;
      hostent->h_addr_list[0] = (char*)malloc(sizeof(struct in_addr));
      if (!hostent->h_addr_list[0])
	break;
      hostent->h_aliases =(char**) malloc((naliases + 1) * sizeof(char *));
      if (!hostent->h_aliases)
	break;

      /* Copy in aliases. */
      naliases = 0;
      if (!end_at_hostname)
	{
	  p = canonical + strlen(canonical) + 1;
	  while (isspace((unsigned char)*p))
	    p++;
	  while (*p)
	    {
	      q = p;
	      while (*q && !isspace((unsigned char)*q))
		q++;
	      hostent->h_aliases[naliases] = (char*) malloc(q - p + 1);
	      if (hostent->h_aliases[naliases] == NULL)
		break;
	      memcpy(hostent->h_aliases[naliases], p, q - p);
	      hostent->h_aliases[naliases][q - p] = 0;
	      p = q;
	      while (isspace((unsigned char)*p))
		p++;
	      naliases++;
	    }
	  if (*p)
	    break;
	}
      hostent->h_aliases[naliases] = NULL;

      hostent->h_addrtype = AF_INET;
      hostent->h_length = sizeof(struct in_addr);
      memcpy(hostent->h_addr_list[0], &addr, sizeof(struct in_addr));
      hostent->h_addr_list[1] = NULL;
      *host = hostent;
      free(line);
      return ARES_SUCCESS;
    }
  free(line);

  if (status == ARES_SUCCESS)
    {
      /* Memory allocation failure; clean up. */
      if (hostent)
	{
	  free((char *) hostent->h_name);
	  if (hostent->h_aliases)
	    {
	      for (alias = hostent->h_aliases; *alias; alias++)
		free(*alias);
	    }
	  free(hostent->h_aliases);
	  if (hostent->h_addr_list)
	    free(hostent->h_addr_list[0]);
	  free(hostent->h_addr_list);
	}
      free(hostent);
      return ARES_ENOMEM;
    }

  return status;
}
