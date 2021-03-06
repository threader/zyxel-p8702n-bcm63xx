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

static const char rcsid[] = "$Id: ares_process.c,v 1.3.2.1 2007/11/21 09:23:59 gur05066 Exp $";



#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include "ares.h"
#include "ares_dns.h"
#include "ares_private.h"
#include "sdfstorage_macro.h"
#include "icf_common_types.h"

static void write_tcp_data(ares_channel channel, fd_set *write_fds,
			   time_t now);
static void read_tcp_data(ares_channel channel, fd_set *read_fds, time_t now);
static void read_udp_packets(ares_channel channel, fd_set *read_fds,
			     time_t now);
static void process_timeouts(ares_channel channel, time_t now);
static void process_answer(ares_channel channel, unsigned char *abuf,
			   int alen, int whichserver, int tcp, int now);
static void handle_error(ares_channel channel, int whichserver, time_t now);
static void next_server(ares_channel channel, struct query *query, time_t now);
static int open_tcp_socket(ares_channel channel, struct server_state *server);
static int open_udp_socket(ares_channel channel, struct server_state *server);
static int same_questions(const unsigned char *qbuf, int qlen,
			  const unsigned char *abuf, int alen);
static void end_query(ares_channel channel, struct query *query, int status,
		      unsigned char *abuf, int alen);

/* Something interesting happened on the wire, or there was a timeout.
 * See what's up and respond accordingly.
 */

extern icf_glb_pdb_st *p_persistent_glb_pdb;
extern icf_return_t icf_dbm_get_module_glb_data(
        INOUT icf_glb_pdb_st        *p_glb_pdb,
        IN    icf_int_module_id_t   module_id,
        OUT   icf_void_t            **p_p_data,
        OUT   icf_error_t           *p_ecode);

void ares_process(ares_channel channel, fd_set *read_fds, fd_set *write_fds)
{
  time_t now;

  time(&now);
  write_tcp_data(channel, write_fds, now);
  read_tcp_data(channel, read_fds, now);
  read_udp_packets(channel, read_fds, now);
  process_timeouts(channel, now);
}

/* If any TCP sockets select true for writing, write out queued data
 * we have for them.
 */
static void write_tcp_data(ares_channel channel, fd_set *write_fds, time_t now)
{
  struct server_state *server;
  struct send_request *sendreq;
  struct iovec *vec;
  int i, n, count;

  for (i = 0; i < channel->nservers; i++)
    {
      /* Make sure server has data to send and is selected in write_fds. */
      server = &channel->servers[i];
      if (!server->qhead || server->tcp_socket == -1
	  || !FD_ISSET(server->tcp_socket, write_fds))
	continue;

      /* Count the number of send queue items. */
      n = 0;
      for (sendreq = server->qhead; sendreq; sendreq = sendreq->next)
	n++;

      /* Allocate iovecs so we can send all our data at once. */
      vec = (struct iovec*) malloc(n * sizeof(struct iovec));
      if (vec)
	{
	  /* Fill in the iovecs and send. */
	  n = 0;
	  for (sendreq = server->qhead; sendreq; sendreq = sendreq->next)
	    {
	      vec[n].iov_base = (char *) sendreq->data;
	      vec[n].iov_len = sendreq->len;
	      n++;
	    }
	  count = writev(server->tcp_socket, vec, n);
	  free(vec);
	  if (count < 0)
	    {
	      handle_error(channel, i, now);
	      continue;
	    }

	  /* Advance the send queue by as many bytes as we sent. */
	  while (count)
	    {
	      sendreq = server->qhead;
	      if (count >= sendreq->len)
		{
		  count -= sendreq->len;
		  server->qhead = sendreq->next;
		  if (server->qhead == NULL)
		    server->qtail = NULL;
		  free(sendreq);
		}
	      else
		{
		  sendreq->data += count;
		  sendreq->len -= count;
		  break;
		}
	    }
	}
      else
	{
	  /* Can't allocate iovecs; just send the first request. */
	  sendreq = server->qhead;
	  count = write(server->tcp_socket, sendreq->data, sendreq->len);
	  if (count < 0)
	    {
	      handle_error(channel, i, now);
	      continue;
	    }

	  /* Advance the send queue by as many bytes as we sent. */
	  if (count == sendreq->len)
	    {
	      server->qhead = sendreq->next;
	      if (server->qhead == NULL)
		server->qtail = NULL;
	      free(sendreq);
	    }
	  else
	    {
	      sendreq->data += count;
	      sendreq->len -= count;
	    }
	}
    }
}

/* If any TCP socket selects true for reading, read some data,
 * allocate a buffer if we finish reading the length word, and process
 * a packet if we finish reading one.
 */
static void read_tcp_data(ares_channel channel, fd_set *read_fds, time_t now)
{
  struct server_state *server;
  int i, count;

  for (i = 0; i < channel->nservers; i++)
    {
      /* Make sure the server has a socket and is selected in read_fds. */
      server = &channel->servers[i];
      if (server->tcp_socket == -1 || !FD_ISSET(server->tcp_socket, read_fds))
	continue;

      if (server->tcp_lenbuf_pos != 2)
	{
	  /* We haven't yet read a length word, so read that (or
	   * what's left to read of it).
	   */
	  count = read(server->tcp_socket,
		       server->tcp_lenbuf + server->tcp_lenbuf_pos,
		       2 - server->tcp_lenbuf_pos);
	  if (count <= 0)
	    {
	      handle_error(channel, i, now);
	      continue;
	    }

	  server->tcp_lenbuf_pos += count;
	  if (server->tcp_lenbuf_pos == 2)
	    {
	      /* We finished reading the length word.  Decode the
               * length and allocate a buffer for the data.
	       */
	      server->tcp_length = server->tcp_lenbuf[0] << 8
		| server->tcp_lenbuf[1];
	      server->tcp_buffer = (unsigned char*)malloc(server->tcp_length);
	      if (!server->tcp_buffer)
		handle_error(channel, i, now);
	      server->tcp_buffer_pos = 0;
	    }
	}
      else
	{
	  /* Read data into the allocated buffer. */
	  count = read(server->tcp_socket,
		       server->tcp_buffer + server->tcp_buffer_pos,
		       server->tcp_length - server->tcp_buffer_pos);
	  if (count <= 0)
	    {
	      handle_error(channel, i, now);
	      continue;
	    }

	  server->tcp_buffer_pos += count;
	  if (server->tcp_buffer_pos == server->tcp_length)
	    {
	      /* We finished reading this answer; process it and
               * prepare to read another length word.
	       */
	      process_answer(channel, server->tcp_buffer, server->tcp_length,
			     i, 1, now);
	      free(server->tcp_buffer);
	      server->tcp_buffer = NULL;
	      server->tcp_lenbuf_pos = 0;
	    }
	}
    }
}

/* If any UDP sockets select true for reading, process them. */
static void read_udp_packets(ares_channel channel, fd_set *read_fds,
			     time_t now)
{
  struct server_state *server;
  int i, count;
  unsigned char buf[PACKETSZ + 1];

  for (i = 0; i < channel->nservers; i++)
    {
      /* Make sure the server has a socket and is selected in read_fds. */
      server = &channel->servers[i];
      if (server->udp_socket == -1 || !FD_ISSET(server->udp_socket, read_fds))
	continue;

      count = recv(server->udp_socket, buf, sizeof(buf), 0);
      if (count <= 0)
	handle_error(channel, i, now);

      process_answer(channel, buf, count, i, 0, now);
    }
}

/* If any queries have timed out, note the timeout and move them on. */
static void process_timeouts(ares_channel channel, time_t now)
{
  struct query *query, *next;

  for (query = channel->queries; query; query = next)
    {
      next = query->next;
      if (query->timeout != 0 && now >= query->timeout)
	{
	  query->error_status = ARES_ETIMEOUT;
	  next_server(channel, query, now);
	}
    }
}

/* Handle an answer from a server. */
static void process_answer(ares_channel channel, unsigned char *abuf,
			   int alen, int whichserver, int tcp, int now)
{
  int id, tc, rcode;
  struct query *query;

  /* If there's no room in the answer for a header, we can't do much
   * with it. */
  if (alen < HFIXEDSZ)
    return;

  /* Grab the query ID, truncate bit, and response code from the packet. */
  id = DNS_HEADER_QID(abuf);
  tc = DNS_HEADER_TC(abuf);
  rcode = DNS_HEADER_RCODE(abuf);

  /* Find the query corresponding to this packet. */
  for (query = channel->queries; query; query = query->next)
    {
      if (query->qid == id)
	break;
    }
  if (!query)
    return;

  /* If we got a truncated UDP packet and are not ignoring truncation,
   * don't accept the packet, and switch the query to TCP if we hadn't
   * done so already.
   */
  if ((tc || alen > PACKETSZ) && !tcp && !(channel->flags & ARES_FLAG_IGNTC))
    {
      if (!query->using_tcp)
	{
	  query->using_tcp = 1;
	  ares__send_query(channel, query, now);
	}
      return;
    }

  /* Limit alen to PACKETSZ if we aren't using TCP (only relevant if we
   * are ignoring truncation.
   */
  if (alen > PACKETSZ && !tcp)
    alen = PACKETSZ;

  /* If we aren't passing through all error packets, discard packets
   * with SERVFAIL, NOTIMP, or REFUSED response codes.
   */
  if (!(channel->flags & ARES_FLAG_NOCHECKRESP))
    {
      if (rcode == SERVFAIL || rcode == NOTIMP || rcode == REFUSED)
	{
	  query->skip_server[whichserver] = 1;
	  if (query->server == whichserver)
	    next_server(channel, query, now);
	  return;
	}
      if (!same_questions((unsigned char*)query->qbuf, query->qlen, abuf, alen))
	{
	  if (query->server == whichserver)
	    next_server(channel, query, now);
	  return;
	}
    }

  end_query(channel, query, ARES_SUCCESS, abuf, alen);
}

static void handle_error(ares_channel channel, int whichserver, time_t now)
{
  struct query *query;
  /* Shetti: FMR fix related, done for CERT specific testing. */
  struct query *tmp_query;

  /* Reset communications with this server. */
  ares__close_sockets(&channel->servers[whichserver]);

  /* Tell all queries talking to this server to move on and not try
   * this server again.
   */
  for (query = channel->queries; query; )
    {
		tmp_query = query;
		query = query->next;
      if (tmp_query->server == whichserver)
	{
	  tmp_query->skip_server[whichserver] = 1;
	  next_server(channel, tmp_query, now);
	}
    }
}

static void next_server(ares_channel channel, struct query *query, time_t now)
{
  /* Advance to the next server or try. */
  query->server++;
  for (; query->qtry < channel->tries; query->qtry++)
    {
      for (; query->server < channel->nservers; query->server++)
	{
	  if (!query->skip_server[query->server])
	    {
	      ares__send_query(channel, query, now);
	      return;
	    }
	}
      query->server = 0;

      /* Only one try if we're using TCP. */
      if (query->using_tcp)
	break;
    }
  end_query(channel, query, query->error_status, NULL, 0);
}

void ares__send_query(ares_channel channel, struct query *query, time_t now)
{
  struct send_request *sendreq;
  struct server_state *server;

  server = &channel->servers[query->server];

  if (query->using_tcp)
    {
      /* Make sure the TCP socket for this server is set up and queue
       * a send request.
       */
      if (server->tcp_socket == -1)
	{
	  if (open_tcp_socket(channel, server) == -1)
	    {
	      query->skip_server[query->server] = 1;
	      next_server(channel, query, now);
	      return;
	    }
	}
      sendreq = (struct send_request *) malloc(sizeof(struct send_request));
      if (!sendreq)
	end_query(channel, query, ARES_ENOMEM, NULL, 0);
      sendreq->data = query->tcpbuf;
      sendreq->len = query->tcplen;
      sendreq->next = NULL;
      if (server->qtail)
	server->qtail->next = sendreq;
      else
	server->qhead = sendreq;
      server->qtail = sendreq;
      query->timeout = 0;
    }
  else
    {
      if (server->udp_socket == -1)
	{
	  if (open_udp_socket(channel, server) == -1)
	    {
	      query->skip_server[query->server] = 1;
	      next_server(channel, query, now);
	      return;
	    }
	}
      if (send(server->udp_socket, query->qbuf, query->qlen, 0) == -1)
	{
	  query->skip_server[query->server] = 1;
	  next_server(channel, query, now);
	  return;
	}
      query->timeout = now
	  + ((query->qtry == 0) ? channel->timeout
	     : channel->timeout << query->qtry / channel->nservers);
    }
}

static int open_tcp_socket(ares_channel channel, struct server_state *server)
{
  int s, flags;
  struct sockaddr_storage sin;
  int socklen = 0;

  /* Acquire a socket. */
  s = socket(channel->networkType, SOCK_STREAM, 0);
  if (s == -1)
    return -1;

  /* Set the socket non-blocking. */
  if (fcntl(s, F_GETFL, &flags) == -1)
    {
      close(s);
      return -1;
    }
  flags |= O_NONBLOCK;
  if (fcntl(s, F_SETFL, flags) == -1)
    {
      close(s);
      return -1;
    }

  /* Connect to the server. */
  memset(&sin, 0, sizeof(sin));
  // IPv6 Support
  ASdf_mc_set_familyIntoSockaddrStorage(&sin,channel->networkType);
  ASdf_mc_set_portIntoSockaddrStorage(&sin,channel->tcp_port);

  if(channel->networkType == AF_INET)
  {
  	ASdf_mc_set_SaddrIntoSockaddrStorage(&sin,(server->addr));
	socklen = sizeof(struct sockaddr_in);
  }
  else if(channel->networkType == AF_INET6)
  {
  	ASdf_mc_set_S6addrIntoSockaddrStorage(&sin,(server->addr6));
	socklen = sizeof(struct sockaddr_in6);
  }

  if (connect(s, (struct sockaddr *) &sin, socklen) == -1
      && errno != EINPROGRESS)
    {
      close(s);
      return -1;
    }

  server->tcp_socket = s;
  return 0;
}

static int open_udp_socket(ares_channel channel, struct server_state *server)
{
  int s;
  struct sockaddr_storage sin;
  int socklen1 = 0;
   
  /* local address where socket will be binded.*/
  struct sockaddr_storage local_addr;
  struct in_addr   sin_addr;

  unsigned short dns_source_port;
  icf_config_data_st  *p_config_data = ICF_NULL;
  icf_error_t         ecode = ICF_ERROR_NONE;  
 
  /* Acquire a socket. */
  s = socket(channel->networkType, SOCK_DGRAM, 0);
  if (s == -1)
    return -1;
  memset(&local_addr, 0, sizeof(local_addr));

  ASdf_mc_set_familyIntoSockaddrStorage(&local_addr,channel->networkType);

  if(channel->networkType == AF_INET)
  {
        sin_addr.s_addr = INADDR_ANY;
  	ASdf_mc_set_SaddrIntoSockaddrStorage(&local_addr,sin_addr);
	socklen1 = sizeof(struct sockaddr_in);
  }

  /* get cfg data for getting the max number of applications*/
  if(ICF_FAILURE == icf_dbm_get_module_glb_data(     
        p_persistent_glb_pdb,
        (icf_int_module_id_t const) ICF_INT_MODULE_CFG,
        (icf_void_t *)&p_config_data,
        &ecode))
  {
       return 0;
  }

  if (ICF_DNS_PORT_CONFIGURED_BY_APP & p_config_data->dns_configuration)
  {
     dns_source_port = p_config_data->dns_source_port;
     ASdf_mc_set_portIntoSockaddrStorage(&local_addr,htons(dns_source_port));

     if (bind(s, (struct sockaddr *) &local_addr, socklen1) == -1)
     {
        close(s);
        return -1;
     }
  }

  /* Connect to the server. */
  memset(&sin, 0, sizeof(sin));
  // IPv6 Support
  ASdf_mc_set_familyIntoSockaddrStorage(&sin,channel->networkType);
  ASdf_mc_set_portIntoSockaddrStorage(&sin,channel->udp_port);

  if(channel->networkType == AF_INET)
  {
  	ASdf_mc_set_SaddrIntoSockaddrStorage(&sin,(server->addr));
	socklen1 = sizeof(struct sockaddr_in);
  }
  else if(channel->networkType == AF_INET6)
  {
  	ASdf_mc_set_S6addrIntoSockaddrStorage(&sin,(server->addr6));
	socklen1 = sizeof(struct sockaddr_in6);
  }

  if (connect(s, (struct sockaddr *) &sin, socklen1) == -1)
    {
	  close(s);
      return -1;
    }

  server->udp_socket = s;
  return 0;
}

static int same_questions(const unsigned char *qbuf, int qlen,
			  const unsigned char *abuf, int alen)
{
  struct {
    const unsigned char *p;
    int qdcount;
    char *name;
    int namelen;
    int type;
    int dnsclass;
  } q, a;
  int i, j;

  if (qlen < HFIXEDSZ || alen < HFIXEDSZ)
    return 0;

  /* Extract qdcount from the request and reply buffers and compare them. */
  q.qdcount = DNS_HEADER_QDCOUNT(qbuf);
  a.qdcount = DNS_HEADER_QDCOUNT(abuf);
  if (q.qdcount != a.qdcount)
    return 0;

  /* For each question in qbuf, find it in abuf. */
  q.p = qbuf + HFIXEDSZ;
  for (i = 0; i < q.qdcount; i++)
    {
      /* Decode the question in the query. */
      if (ares_expand_name(q.p, qbuf, qlen, &q.name, &q.namelen)
	  != ARES_SUCCESS)
	return 0;
      q.p += q.namelen;
      if (q.p + QFIXEDSZ > qbuf + qlen)
	{
	  free(q.name);
	  return 0;
	}
      q.type = DNS_QUESTION_TYPE(q.p);
      q.dnsclass = DNS_QUESTION_CLASS(q.p);
      q.p += QFIXEDSZ;

      /* Search for this question in the answer. */
      a.p = abuf + HFIXEDSZ;
      for (j = 0; j < a.qdcount; j++)
	{
	  /* Decode the question in the answer. */
	  if (ares_expand_name(a.p, abuf, alen, &a.name, &a.namelen)
	      != ARES_SUCCESS)
	    {
	      free(q.name);
	      return 0;
	    }
	  a.p += a.namelen;
	  if (a.p + QFIXEDSZ > abuf + alen)
	    {
	      free(q.name);
	      free(a.name);
	      return 0;
	    }
	  a.type = DNS_QUESTION_TYPE(a.p);
	  a.dnsclass = DNS_QUESTION_CLASS(a.p);
	  a.p += QFIXEDSZ;

	  /* Compare the decoded questions. */
	  if (strcasecmp(q.name, a.name) == 0 && q.type == a.type
	      && q.dnsclass == a.dnsclass)
	    {
	      free(a.name);
	      break;
	    }
	  free(a.name);
	}

      free(q.name);
      if (j == a.qdcount)
	return 0;
    }
  return 1;
}

static void end_query(ares_channel channel, struct query *query, int status,
		      unsigned char *abuf, int alen)
{
  struct query **q;
  int i;

  query->callback(query->arg, status, abuf, alen);
  for (q = &channel->queries; *q; q = &(*q)->next)
    {
      if (*q == query)
	break;
    }
  *q = query->next;
  free(query->tcpbuf);
  free(query->skip_server);
  free(query);

  /* Simple cleanup policy: if no queries are remaining, close all
   * network sockets unless STAYOPEN is set.
   */
  if (!channel->queries && !(channel->flags & ARES_FLAG_STAYOPEN))
    {
      for (i = 0; i < channel->nservers; i++)
	ares__close_sockets(&channel->servers[i]);
    }
}
void free_query(void *AresChannel, int id)
{
   struct query *query, **q;
   ares_channel channel = (ares_channel)AresChannel;

  /* Find the query corresponding to this packet. */
  for (query = channel->queries; query; query = query->next)
    {
      if (query->qid == id)
	break;
    }
 
  if (!query)
    return;

  for (q = &channel->queries; *q; q = &(*q)->next)
  {
      if (*q == query)
	break;
  }
  *q = query->next;

  free_qquery(query->arg);
  
  free(query->tcpbuf);
  free(query->skip_server);
  free(query);
}
