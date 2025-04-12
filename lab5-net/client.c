#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "client.h"

#define BUFFER_MAX_SIZE 4096

/* Given a struct with server information, return a dynamically allocated
   string of the IP address in dotted decimal or colon hexadecimal notation.
   Consult Chapter 4, as well as /usr/include/netdb.h and
   /usr/include/arpa/inet.h as needed. Use inet_ntop() to get the formatted
   string based on the address's ai_addr field.
 */
char *
addr_string (struct addrinfo *server)
{
  // Return safely in case server is NULL:
  if (server == NULL)
    return strdup ("no address information");

  char buffer[INET6_ADDRSTRLEN];
  void *addr = NULL;

  if (server->ai_family == AF_INET)
    {
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)server->ai_addr;
      addr = &(ipv4->sin_addr);
    }
  else if (server->ai_family == AF_INET6)
    {
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)server->ai_addr;
      addr = &(ipv6->sin6_addr);
    }
  else
    {
      return strdup ("error");
    }

  inet_ntop (server->ai_family, addr, buffer, sizeof (buffer));

  return strdup (buffer);
}

/* Given the server address info, return a dynamically allocated string with
   its transport-layer protocol, IP version, and IP address. For instance,
   querying jmu.edu over TCP/IPv4 should return:
     "TCP IPv4 134.126.10.50"
   Use addr_string() to get the formatted address string, concatenate it to
   the string to return, then free the result from addr_string(). If the
   passed server parameter is NULL, use strdup() to return a dynamically
   allocated copy of "no address information".

   NOTE: When distinguishing TCP and UDP, you should be checking the
   server->ai_socktype field, not server->ai_protocol. The test cases do not
   set the server->ai_protocol, which is sometimes not used in practice.
 */
char *
serv_string (struct addrinfo *server)
{
  // Return safely in case server is NULL:
  if (server == NULL)
    return strdup ("no address information");

  // Allocate 10 bytes for protocol (TCP or UDP), IP version (IPv4 or
  // IPv6), along with spaces and the ending null byte. In addition,
  // include enough space for an IP address (either v4 or v6).

  // Now, use strncat() to build the string based on information from
  // the server struct.

  const char *protocol;
  if (server->ai_socktype == SOCK_STREAM)
    {
      protocol = "TCP";
    }
  else if (server->ai_socktype == SOCK_DGRAM)
    {
      protocol = "UDP";
    }
  else
    {
      protocol = "UNKNOWN";
    }

  const char *ip_version;
  if (server->ai_family == AF_INET)
    {
      ip_version = "IPv4";
    }
  else if (server->ai_family == AF_INET6)
    {
      ip_version = "IPv6";
    }
  else
    {
      ip_version = "Unknown";
    }

  char *addr = addr_string (server);
  size_t total_len
      = strlen (protocol) + strlen (ip_version) + strlen (addr) + 3;
  char *result = malloc (total_len);
  snprintf (result, total_len, "%s %s %s", protocol, ip_version, addr);

  free (addr);

  return result;
}

/* Given a hostname string, use getaddrinfo() to query DNS for the specified
   protocol parameters. Boolean values indicate whether or not to use IPv6
   (as opposed to IPv4) or TCP (as opposed to UDP). For this lab, the protocol
   will only be "http", though it could be others in a more complete
   implementation. Return the pointer to the linked list of server results.
 */
struct addrinfo *
get_server_list (const char *hostname, const char *proto, bool tcp, bool ipv6)
{
  struct addrinfo *server_list = NULL;
  struct addrinfo hints;
  memset (&hints, 0, sizeof (hints));

  hints.ai_family = ipv6 ? AF_INET6 : AF_INET;
  hints.ai_socktype = tcp ? SOCK_STREAM : SOCK_DGRAM;
  hints.ai_protocol = tcp ? IPPROTO_TCP : IPPROTO_UDP;
  if (strcmp (hostname, "localhost") == 0)
    {
      hints.ai_flags = AI_PASSIVE;
    }
  else
    {
      hints.ai_flags = AI_V4MAPPED | AI_ADDRCONFIG;
    }

  int status = getaddrinfo (hostname, proto, &hints, &server_list);
  if (status != 0)
    {
      return NULL;
    }
  return server_list;
}
