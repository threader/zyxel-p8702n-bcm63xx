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

static const char rcsid[] = "$Id: ares_free_hostent.c,v 1.2 2006/08/10 13:02:56 umsingh Exp $";

#include <stdlib.h>
#include <netdb.h>
#include "ares.h"

void ares_free_hostent(struct hostent *host)
{
  char **p;

  free(host->h_name);
  for (p = host->h_aliases; *p; p++)
    free(*p);
  free(host->h_aliases);
  free(host->h_addr_list[0]);
  free(host->h_addr_list);
  free(host);
}
