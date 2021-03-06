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

static const char rcsid[] = "$Id: ares_strerror.c,v 1.2 2006/08/10 13:02:59 umsingh Exp $";

#include <assert.h>
#include "ares.h"

const char *ares_strerror(int code, char **memptr)
{

  const char *errtext[] = {
    "Successful completion",
    "DNS server returned answer with no data",
    "DNS server claims query was misformatted",
    "DNS server returned general failure",
    "Domain name not found",
    "DNS server does not implement requested operation",
    "DNS server refused query",
    "Misformatted DNS query",
    "Misformatted domain name",
    "Unsupported address family",
    "Misformatted DNS reply",
    "Could not contact DNS servers",
    "Timeout while contacting DNS servers",
    "End of file",
    "Error reading file",
    "Out of memory"
  };
  assert(code >= 0 && (unsigned int) code < (sizeof(errtext) / sizeof(*errtext)));
  /* A future implementation may want to handle internationalization.
   * For now, just return a string literal from a table.
   */
  memptr = memptr;
  return errtext[code];
}
