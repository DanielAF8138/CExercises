#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "pipe.h"

int cmdline (int, char **, bool *, char **, char **, size_t *);

void
usage (void)
{
  printf ("Usage: ipc [options] EXEC ARG\n");
  printf (" Send ARG through a pipe to child process that runs EXEC.\n");
  printf (" Option is:\n");
  printf ("  -s      Use posix_spawn() instead of fork() and exec().\n");
}

int
main (int argc, char **argv)
{
  bool use_spawn = false;
  char *exec = NULL;
  char *fname = NULL;
  size_t lineno = -1;

  if (cmdline (argc, argv, &use_spawn, &exec, &fname, &lineno) < 0)
    {
      usage ();
      return EXIT_FAILURE;
    }

  // Use fork/exec or spawn to run the executable exec
  char *sum = NULL;
  if (!use_spawn)
    sum = get_result (exec, fname);
  else
    sum = spawn_result (exec, fname);

  if (sum == NULL)
    return EXIT_FAILURE;

  if (sum == NULL)
    sum = strdup ("ERROR: File does not exist");
  printf ("%s(%s) = '%s'\n", exec, fname, sum);

  // If you get an error here, it's because sum is not dynamically
  // allocated correctly.
  free (sum);
  return EXIT_SUCCESS;
}

/* Helper for parsing the command line arguments. Sets spawn,
   filename, and lineno as call-by-reference parameters. Return 0
   on success, -1 otherwise. */
int
cmdline (int argc, char **argv, bool *spawn, char **exec, char **filename,
         size_t *lineno)
{
  int option;

  while ((option = getopt (argc, argv, "sh")) != -1)
    {
      switch (option)
        {
        case 's':
          *spawn = true;
          break;
        case 'h':
          return -1;
          break;
        default:
          return -1;
        }
    }

  if (optind + 2 != argc)
    return -1;

  *exec = argv[optind];
  *filename = argv[optind + 1];

  return 0;
}
