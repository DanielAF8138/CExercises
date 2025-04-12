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

#include "port_utils.h"
#include "server.h"
#include "utils.h"

int cmdline (int, char **, char **);

void
usage (void)
{
  printf ("Usage: web [options]\n");
  printf (" Option is:\n");
  printf ("  -p P    Use application protocol P instead of default port\n");
  printf ("\n Possible requests are:\n");
  printf ("  \"GET / HTTP/1.0\"\n");
  printf ("  \"GET /index.html HTTP/1.0\"\n");
  printf ("  \"GET /bootstrap.html HTTP/1.0\"\n");
}

int
main (int argc, char **argv)
{
  char *protocol = get_port ();
  if (cmdline (argc, argv, &protocol) < 0)
    {
      usage ();
      return EXIT_FAILURE;
    }

  ssize_t bytes = serve_web (protocol);
  printf ("Bytes written to socket: %zd\n", bytes);

  return EXIT_SUCCESS;
}

/* DO NOT MODIFY THIS FUNCTION */

int
cmdline (int argc, char **argv, char **protocol)
{
  int option;

  while ((option = getopt (argc, argv, "p:h")) != -1)
    {
      switch (option)
        {
          // Change this to merge -p and -o as same flag
        case 'p':
          *protocol = optarg;
          break;
        case 'h':
          return -1;
          break;
        default:
          return -1;
        }
    }

  return 0;
}
