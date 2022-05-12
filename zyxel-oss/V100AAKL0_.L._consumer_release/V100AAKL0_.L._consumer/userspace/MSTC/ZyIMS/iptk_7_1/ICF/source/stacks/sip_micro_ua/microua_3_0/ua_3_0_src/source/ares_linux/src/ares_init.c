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

static const char rcsid[] = "$Id: ares_init.c,v 1.4.2.1 2007/11/30 14:26:43 gur15444 Exp $";



#include <sys/types.h>
#include <sys/time.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include "ares.h"
#include "ares_private.h"
#ifdef SSF_ARES
#include "sdfdiagnostics.h"
#endif


static int init_by_options(ares_channel channel, struct ares_options *options,
			   int optmask);
static int init_by_environment(ares_channel channel);
static int init_by_resolv_conf(ares_channel channel);
static int init_by_defaults(ares_channel channel);
static int config_domain(ares_channel channel, char *str);
static int config_lookup(ares_channel channel, const char *str);
static int config_nameserver(int networkType, struct server_state **servers, int *nservers,
			     const char *str);
static int config_sortlist(struct apattern **sortlist, int *nsort,
			   const char *str,int networkType);
static int set_search(ares_channel channel, const char *str);
static int set_options(ares_channel channel, const char *str);
static char *try_config(char *s, char *opt);
static const char *try_option(const char *p, const char *q, const char *opt);
static int ip_addr(const char *s, int len, struct in_addr *addr);
static void natural_mask(struct apattern *pat);

int ares_init(ares_channel *channelptr,int networkType)
{
  return ares_init_options(channelptr, NULL, 0, networkType);
}

int ares_init_options(ares_channel *channelptr, struct ares_options *options,
		      int optmask,int networkType)
{
  ares_channel channel;
  int i, status;
  int resolveRetval = 0;
  struct server_state *server;
  struct timeval tv;

    // IPv6 support
  if(networkType != AF_INET && networkType != AF_INET6)
  {
	  // Unspecified family
	  return ARES_EBADFAMILY;
  }

  channel = (struct ares_channeldata *)malloc(sizeof(struct ares_channeldata));
  if (!channel)
    return ARES_ENOMEM;

  // IPv6 support 
  channel->networkType = networkType;

  /* Set everything to distinguished values so we know they haven't
   * been set yet.
   */
  channel->flags = -1;
  channel->timeout = -1;
  channel->tries = -1;
  channel->ndots = -1;
  channel->udp_port = -1;
  channel->tcp_port = -1;
  channel->nservers = -1;
  channel->ndomains = -1;
  channel->nsort = -1;
  channel->lookups = NULL;
  channel->qid = -1;/* It was initialised as NULL in NORTEL BRANCH */

  /* Initialize configuration by each of the four sources, from highest
   * precedence to lowest.
   */
  status = init_by_options(channel, options, optmask);
  if (status == ARES_SUCCESS)
    status = init_by_environment(channel);
  if (status == ARES_SUCCESS)
  {
    status = init_by_resolv_conf(channel);
    resolveRetval = status;
  }
  if (status == ARES_SUCCESS || status == ARES_ECONNREFUSED)
    status = init_by_defaults(channel);
  if (status != ARES_SUCCESS)
    {
      /* Something failed; clean up memory we may have allocated. */
      if (channel->nservers != -1)
	free(channel->servers);
      if (channel->ndomains != -1)
	{
	  for (i = 0; i < channel->ndomains; i++)
	    free(channel->domains[i]);
	  free(channel->domains);
	}
      if (channel->nsort != -1)
	free(channel->sortlist);
      free(channel->lookups);
      free(channel);
      return status;
    }

  /* Trim to one server if ARES_FLAG_PRIMARY is set. */
  if ((channel->flags & ARES_FLAG_PRIMARY) && channel->nservers > 1)
    channel->nservers = 1;

  /* Initialize server states. */
  for (i = 0; i < channel->nservers; i++)
    {
      server = &channel->servers[i];
      server->udp_socket = -1;
      server->tcp_socket = -1;
      server->tcp_lenbuf_pos = 0;
      server->tcp_buffer = NULL;
      server->qhead = NULL;
      server->qtail = NULL;
    }

  /* Choose a somewhat random query ID.  The main point is to avoid
   * collisions with stale queries.  An attacker trying to spoof a DNS
   * answer also has to guess the query ID, but it's only a 16-bit
   * field, so there's not much to be done about that.
   */
  gettimeofday(&tv, NULL);
  channel->next_id = (tv.tv_sec ^ tv.tv_usec ^ getpid()) & 0xffff;

  channel->queries = NULL;

  *channelptr = channel;
  if(0 != resolveRetval)
    return ARES_SUCCESS;
  else
    return resolveRetval;
}

static int init_by_options(ares_channel channel, struct ares_options *options,
			   int optmask)
{
  int i;

  /* Easy stuff. */
  if ((optmask & ARES_OPT_FLAGS) && channel->flags == -1)
    channel->flags = options->flags;
  if ((optmask & ARES_OPT_TIMEOUT) && channel->timeout == -1)
    channel->timeout = options->timeout;
  if ((optmask & ARES_OPT_TRIES) && channel->tries == -1)
    channel->tries = options->tries;
  if ((optmask & ARES_OPT_NDOTS) && channel->ndots == -1)
    channel->ndots = options->ndots;
  if ((optmask & ARES_OPT_UDP_PORT) && channel->udp_port == -1)
    channel->udp_port = options->udp_port;
  if ((optmask & ARES_OPT_TCP_PORT) && channel->tcp_port == -1)
    channel->tcp_port = options->tcp_port;

  /* Copy the servers, if given. */
  if ((optmask & ARES_OPT_SERVERS) && channel->nservers == -1)
    {
      channel->servers =(struct server_state *)
	malloc(options->nservers * sizeof(struct server_state));
      if (!channel->servers && options->nservers != 0)
	return ARES_ENOMEM;
      for (i = 0; i < options->nservers; i++)
	channel->servers[i].addr = options->servers[i];
      channel->nservers = options->nservers;
    }

  /* Copy the domains, if given.  Keep channel->ndomains consistent so
   * we can clean up in case of error.
   */
  if ((optmask & ARES_OPT_DOMAINS) && channel->ndomains == -1)
    {
      channel->domains = (char**)malloc(options->ndomains * sizeof(char *));
      if (!channel->domains && options->ndomains != 0)
	return ARES_ENOMEM;
      for (i = 0; i < options->ndomains; i++)
	{
	  channel->ndomains = i;
	  channel->domains[i] = strdup(options->domains[i]);
	  if (!channel->domains[i])
	    return ARES_ENOMEM;
	}
      channel->ndomains = options->ndomains;
    }

  /* Set lookups, if given. */
  if ((optmask & ARES_OPT_LOOKUPS) && !channel->lookups)
    {
      channel->lookups = strdup(options->lookups);
      if (!channel->lookups)
	return ARES_ENOMEM;
    }

  return ARES_SUCCESS;
}

static int init_by_environment(ares_channel channel)
{
  const char *localdomain, *res_options;
  int status;

  localdomain = getenv("LOCALDOMAIN");
  if (localdomain && channel->ndomains == -1)
    {
      status = set_search(channel, localdomain);
      if (status != ARES_SUCCESS)
	return status;
    }

  res_options = getenv("RES_OPTIONS");
  if (res_options)
    {
      status = set_options(channel, res_options);
      if (status != ARES_SUCCESS)
	return status;
    }

  return ARES_SUCCESS;
}

static int init_by_resolv_conf(ares_channel channel)
{
  FILE *fp;
  char *line = NULL, *p;
  int linesize, status, nservers = 0, nsort = 0;
  struct server_state *servers = NULL;
  struct apattern *sortlist = NULL;

  fp = fopen(PATH_RESOLV_CONF, "r");
  if (!fp)
    return (errno == ENOENT) ? ARES_SUCCESS : ARES_EFILE;
  while ((status = ares__read_line(fp, &line, &linesize)) == ARES_SUCCESS)
    {
      if ((p = try_config(line, (char *)"domain")) && channel->ndomains == -1)
	status = config_domain(channel, p);
      else if ((p = try_config(line, (char *)"lookup")) && !channel->lookups)
	status = config_lookup(channel, p);
      else if ((p = try_config(line, (char *)"search")) && channel->ndomains\
		== -1)
	status = set_search(channel, p);
      else if ((p = try_config(line, (char *)"nameserver")) && channel->nservers == -1)
	status = config_nameserver(channel->networkType,&servers, &nservers, p);
      else if ((p = try_config(line, (char *)"sortlist")) && channel->nsort \
		== -1)
	status = config_sortlist(&sortlist, &nsort, p,channel->networkType);
      else if ((p = try_config(line, (char *)"options")))
	status = set_options(channel, p);
      else
	status = ARES_SUCCESS;
      if (status != ARES_SUCCESS)
	break;
    }
  free(line);
  fclose(fp);

  /* Handle errors. */
  if (status != ARES_EOF)
    {
      free(servers);
      free(sortlist);
      return status;
    }

  /* If we got any name server entries, fill them in. */
  if (servers)
    {
      channel->servers = servers;
      channel->nservers = nservers;
    }

  /* If we got any sortlist entries, fill them in. */
  if (sortlist)
    {
      channel->sortlist = sortlist;
      channel->nsort = nsort;
    }

    // Return could not contact DNS servers
    if(channel->nservers <= 0)
	{
#ifdef SSF_ARES
        Sdf_cl_Diagnostics::getInstance().errorReporter(Sdf_en_lookUp,
               Sdf_en_ConfigurationError,Sdf_en_levelMajorError,
               Sdf_en_errorInvalidAttr,"\n None of the Name servers are "
               "compatible with selected Network Type \n");
#endif
		if(channel->networkType == AF_INET)
			printf("type AF_INET\n"); 
		else
			printf("type AF_INET6\n"); 
		return ARES_ECONNREFUSED;
	}


  return ARES_SUCCESS;
}

static int init_by_defaults(ares_channel channel)
{
  char hostname[MAXHOSTNAMELEN + 1];
  #ifdef SDF_IPV6
      struct in6_addr ipv6; 
  #endif



  if (channel->flags == -1)
    channel->flags = 0;
  if (channel->timeout == -1)
    channel->timeout = DEFAULT_TIMEOUT;
  if (channel->tries == -1)
    channel->tries = DEFAULT_TRIES;
  if (channel->ndots == -1)
    channel->ndots = 1;
  if (channel->udp_port == -1)
    channel->udp_port = htons(NAMESERVER_PORT);
  if (channel->tcp_port == -1)
    channel->tcp_port = htons(NAMESERVER_PORT);

  if (channel->nservers == -1)
    {
      /* If nobody specified servers, try a local named. */
      channel->servers = (struct server_state*)\
	  		malloc(sizeof(struct server_state));
      if (!channel->servers)
	return ARES_ENOMEM;
	  // IPv6 Support
	  if(channel->networkType == AF_INET)
      	   channel->servers[0].addr.s_addr = htonl(INADDR_LOOPBACK);
#ifdef SDF_IPV6
	  else
	  {
		  ipv6 = channel->servers[0].addr6; 
		  if (inet_pton (AF_INET6, "::1", &(ipv6.s6_addr)) == 1)
		  {
    		  printf(" inet_pton is successful\n");
		  }
	  }
#endif /* SDF_IPV6 */

      channel->nservers = 1;
    }

  if (channel->ndomains == -1)
    {
      /* Derive a default domain search list from the kernel hostname,
       * or set it to empty if the hostname isn't helpful.
       */
      if (gethostname(hostname, sizeof(hostname)) == -1
	  || !strchr(hostname, '.'))
	{
	  channel->domains = (char**)malloc(0);
	  channel->ndomains = 0;
	}
      else
	{
	  channel->domains = (char**)malloc(sizeof(char *));
	  if (!channel->domains)
	    return ARES_ENOMEM;
	  channel->ndomains = 0;
	  channel->domains[0] = strdup(strchr(hostname, '.') + 1);
	  if (!channel->domains[0])
	    return ARES_ENOMEM;
	  channel->ndomains = 1;
	}
    }

  if (channel->nsort == -1)
    {
      channel->sortlist = NULL;
      channel->nsort = 0;
    }

  if (!channel->lookups)
    {
      channel->lookups = strdup("bf");
      if (!channel->lookups)
	return ARES_ENOMEM;
    }

  return ARES_SUCCESS;
}

static int config_domain(ares_channel channel, char *str)
{
  char *q;

  /* Set a single search domain. */
  q = str;
  while (*q && !isspace((unsigned char)*q))
    q++;
  *q = 0;
  return set_search(channel, str);
}

static int config_lookup(ares_channel channel, const char *str)
{
  char lookups[3], *l;
  const char *p;

  /* Set the lookup order.  Only the first letter of each work
   * is relevant, and it has to be "b" for DNS or "f" for the
   * host file.  Ignore everything else.
   */
  l = lookups;
  p = str;
  while (*p)
    {
      if ((*p == 'b' || *p == 'f') && l < lookups + 2)
	*l++ = *p;
      while (*p && !isspace((unsigned char)*p))
	p++;
      while (isspace((unsigned char)*p))
	p++;
    }
  *l = 0;
  channel->lookups = strdup(lookups);
  return (channel->lookups) ? ARES_SUCCESS : ARES_ENOMEM;
}

static int config_nameserver(int networkType, struct server_state **servers, int *nservers,
			     const char *str)
{
  struct in_addr addr;
  // IPv6 Support
  struct in6_addr addr6;
  struct server_state *newserv;
  char *tmpstr = NULL;
  char info_str[300];
  info_str[0]='\0';

  /* Add a nameserver entry, if this is a valid address. */
  // IPv6 Support
 if(networkType == AF_INET)
 { 
  	addr.s_addr = inet_addr(str);
  	if (addr.s_addr == INADDR_NONE)
    {
        sprintf(info_str,"%s %s %s","\n Name server ",str," address type" \
                " are incompatible with selected Network Type \n");
#ifdef SSF_ARES
        Sdf_cl_Diagnostics::getInstance().errorReporter(Sdf_en_lookUp,
               Sdf_en_ConfigurationError,Sdf_en_levelMajorError,
               Sdf_en_errorInvalidAttr,info_str);
#endif
    	return ARES_SUCCESS;
    }
 }
 else
 {
	// remove Blank Spaces
	tmpstr = strdup(str);
	tmpstr = strtok(tmpstr," ");

	 if (inet_pton (AF_INET6, tmpstr, &(addr6.s6_addr)) != 1)
     {
        sprintf(info_str,"%s %s %s","\n Name server ",str," address type" \
                " are incompatible with selected Network Type \n");
#ifdef SSF_ARES
        Sdf_cl_Diagnostics::getInstance().errorReporter(Sdf_en_lookUp,
               Sdf_en_ConfigurationError,Sdf_en_levelMajorError,
               Sdf_en_errorInvalidAttr,info_str);
#endif
     	free(tmpstr);
    	return ARES_SUCCESS;
     }

     free(tmpstr);
 }

  newserv = (struct server_state*)realloc(*servers, (*nservers + 1) * sizeof(struct server_state));
  if (!newserv)
    return ARES_ENOMEM;

  // IPv6 Support
  if(networkType == AF_INET)
      newserv[*nservers].addr = addr;
  else
	  newserv[*nservers].addr6 = addr6;

  *servers = newserv;
  (*nservers)++;
  return ARES_SUCCESS;
}

static int config_sortlist(struct apattern **sortlist, int *nsort,
			   const char *str,int networkType)
{
  struct apattern pat, *newsort;
  const char *q;

  // Since the sortlist was used by gethostbyname() or gethostbyaddr()
  // Both functions are IPv4 only compatible and are asynchronous
  // For IPv6 no need to do the sorting of the given list
  if(networkType == AF_INET6)
	  return ARES_SUCCESS;

  /* Add sortlist entries. */
  while (*str && *str != ';')
    {
      q = str;
      while (*q && *q != '/' && *q != ';' && !isspace((unsigned char)*q))
	q++;
      if (ip_addr(str, q - str, &pat.addr) == 0)
	{
	  /* We have a pattern address; now determine the mask. */
	  if (*q == '/')
	    {
	      str = q + 1;
	      while (*q && *q != ';' && !isspace((unsigned char)*q))
		q++;
	      if (ip_addr(str, q - str, &pat.mask) != 0)
		natural_mask(&pat);
	    }
	  else
	    natural_mask(&pat);

	  /* Add this pattern to our list. */
	  newsort = (struct apattern*) \
	  		realloc(*sortlist, (*nsort + 1) * sizeof(struct apattern));
	  if (!newsort)
	    return ARES_ENOMEM;
	  newsort[*nsort] = pat;
	  *sortlist = newsort;
	  (*nsort)++;
	}
      else
	{
	  while (*q && *q != ';' && !isspace((unsigned char)*q))
	    q++;
	}
      str = q;
      while (isspace((unsigned char)*str))
	str++;
    }

  return ARES_SUCCESS;
}

static int set_search(ares_channel channel, const char *str)
{
  int n;
  const char *p, *q;

  /* Count the domains given. */
  n = 0;
  p = str;
  while (*p)
    {
      while (*p && !isspace((unsigned char)*p))
	p++;
      while (isspace((unsigned char)*p))
	p++;
      n++;
    }

  channel->domains = (char**) malloc(n * sizeof(char *));
  if (!channel->domains && n)
    return ARES_ENOMEM;

  /* Now copy the domains. */
  n = 0;
  p = str;
  while (*p)
    {
      channel->ndomains = n;
      q = p;
      while (*q && !isspace((unsigned char)*q))
	q++;
      channel->domains[n] = (char*)malloc(q - p + 1);
      if (!channel->domains[n])
	return ARES_ENOMEM;
      memcpy(channel->domains[n], p, q - p);
      channel->domains[n][q - p] = 0;
      p = q;
      while (isspace((unsigned char)*p))
	p++;
      n++;
    }
  channel->ndomains = n;

  return ARES_SUCCESS;
}

static int set_options(ares_channel channel, const char *str)
{
  const char *p, *q, *val;

  p = str;
  while (*p)
    {
      q = p;
      while (*q && !isspace((unsigned char)*q))
	q++;
      val = try_option(p, q, "ndots:");
      if (val && channel->ndots == -1)
	channel->ndots = atoi(val);
      val = try_option(p, q, "retrans:");
      if (val && channel->timeout == -1)
	channel->timeout = atoi(val);
      val = try_option(p, q, "retry:");
      if (val && channel->tries == -1)
	channel->tries = atoi(val);
      p = q;
      while (isspace((unsigned char)*p))
	p++;
    }

  return ARES_SUCCESS;
}

static char *try_config(char *s, char *opt)
{
  int len;

  len = strlen(opt);
  if (strncmp(s, opt, len) != 0 || !isspace((unsigned char)s[len]))
    return NULL;
  s += len;
  while (isspace((unsigned char)*s))
    s++;
  return s;
}

static const char *try_option(const char *p, const char *q, const char *opt)
{
  int len;

  len = strlen(opt);
  return (q - p > len && strncmp(p, opt, len) == 0) ? p + len : NULL;
}

static int ip_addr(const char *s, int len, struct in_addr *addr)
{
  char ipbuf[16];

  /* Four octets and three periods yields at most 15 characters. */
  if (len > 15)
    return -1;
  memcpy(ipbuf, s, len);
  ipbuf[len] = 0;

  addr->s_addr = inet_addr(ipbuf);
  if (addr->s_addr == INADDR_NONE && strcmp(ipbuf, "255.255.255.255") != 0)
    return -1;

  return 0;
}

static void natural_mask(struct apattern *pat)
{
  struct in_addr addr;

  /* Store a host-byte-order copy of pat in a struct in_addr.  Icky,
   * but portable.
   */
  addr.s_addr = ntohl(pat->addr.s_addr);

  /* This is out of date in the CIDR world, but some people might
   * still rely on it.
   */
  if (IN_CLASSA(addr.s_addr))
    pat->mask.s_addr = htonl(IN_CLASSA_NET);
  else if (IN_CLASSB(addr.s_addr))
    pat->mask.s_addr = htonl(IN_CLASSB_NET);
  else
    pat->mask.s_addr = htonl(IN_CLASSC_NET);
}
