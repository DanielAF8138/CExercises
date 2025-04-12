#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "movies.h"

int cmdline (int, char **, bool *);

void
usage (void)
{
  printf ("Usage: ptrs [-hm]\n");
  printf (" Options are:\n");
  printf ("  -m      Parse the CSV data as a movie\n");
  printf ("  -h      Print this help message\n");
  printf ("Only one option is allowable\n");
}

int
main (int argc, char **argv)
{
  bool run_movie = false;

  if (cmdline (argc, argv, &run_movie) != 0)
    return EXIT_FAILURE;

  if (!run_movie)
    {
      usage ();
      return EXIT_FAILURE;
    }

  // Split the following comma-separated value (CSV) data into fields
  char *data = "The Shawshank Redemption,1994,Drama";
  movie_t movie = split_data (data);
  printf ("Title: %s\n", movie.title);
  printf ("Year: %d\n", movie.year);
  printf ("Genre: %s\n", movie.genre);

  char *rebuilt = merge_data (movie);
  printf ("%s\n", rebuilt);

  free (rebuilt);
  free (movie.title);
  free (movie.genre);

  return EXIT_SUCCESS;
}

/* DO NOT MODIFY THIS FUNCTION. Parses the command line arguments to get
   the signal numbers to override, the number of overridden signals, and
   the order of signals to raise in the full implementation. */
int
cmdline (int argc, char **argv, bool *movies)
{
  int option;

  if ((option = getopt (argc, argv, "mh")) == -1)
    {
      usage ();
      return -1;
    }

  switch (option)
    {
    case 'h':
      *movies = false;
      return -1;
    case 'm':
      *movies = true;
      break;
    default:
      printf ("Invalid command-line option: %c\n", optopt);
      usage ();
      return -1;
    }

  return 0;
}
