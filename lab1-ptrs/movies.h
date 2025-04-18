#ifndef __CS361_MOVIES_H__
#define __CS361_MOVIES_H__

#include <stdbool.h>

typedef struct movie
{
  char *title;
  int year;
  char *genre;
} movie_t;

movie_t split_data (char *);
char *merge_data (movie_t);

#endif
