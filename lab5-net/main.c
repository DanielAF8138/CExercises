/*
 * CS 361: Template project driver
 *
 * Name:
 */

#include <assert.h>
#include <getopt.h>
#include <inttypes.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "client.h"
  
int cmdline (int, char **, char **, bool *, short int *);

void
usage (void)
{
  printf ("Usage: net [options] domain\n");
  printf (" Options are:\n");
  printf ("  -6      Use IPv6 addresses\n");
  printf ("  -p P    Lookup address for application protocol P\n");
}

int
main (int argc, char **argv)
{
  char *protocol = "http";
  bool ipv6 = false;
  short int port = 0;
  if (cmdline (argc, argv, &protocol, &ipv6, &port) < 0)
    {
      usage ();
      return EXIT_FAILURE;
    }
  char *domain = argv[optind];

  // MINIMUM requirements:
  //   Get the server list for the specified domain and protocol
  //   Given this command-line:
  //     ./net -p http www.jmu.edu
  //   Print the following output:
  //     www.jmu.edu http: TCP IPv4 134.126.10.50
  //   The requested transport-layer protocol should be based on the
  //   requested application-layer protocol. TCP is default, but use
  //   UDP for protocols "53", "67", "dns", or "dhcp".
  bool tcp = true;
  if (strcmp (protocol, "53") == 0 || strcmp (protocol, "67") == 0
      || strcmp (protocol, "dns") == 0 || strcmp (protocol, "dhcp") == 0)
    {
      tcp = false;
    }

  struct addrinfo *server_list = get_server_list (domain, protocol, tcp, ipv6);
  if (!server_list)
    {
      fprintf (stderr, "error for %s\n", domain);
      return EXIT_FAILURE;
    }

  char *server_info = serv_string (server_list);
  printf ("%s %s: %s\n", domain, protocol, server_info);

  free (server_info);
  freeaddrinfo (server_list);

  return EXIT_SUCCESS;
}

/* DO NOT MODIFY THIS FUNCTION */

int
cmdline (int argc, char **argv, char **protocol, bool *ipv6, short int *port)
{
  int option;

  while ((option = getopt (argc, argv, "p:6sh")) != -1)
    {
      switch (option)
        {
          // Change this to merge -p and -o as same flag
        case 'p':
          *protocol = optarg;
          break;
        case '6':
          *ipv6 = true;
          break;
        case 'h':
          return -1;
          break;
        default:
          return -1;
        }
    }

  if (optind != argc - 1)
    return -1;

  return 0;
}
