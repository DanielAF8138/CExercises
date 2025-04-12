#define _POSIX_C_SOURCE 200809L // needed for strdup extension

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "movies.h"

/* Given a string (possibly read-only) of IMDB movie data
   (formatted as "Title,Year,Genre"), create a movie_t object
   based on the appropriate fields. */
movie_t
split_data (char *csv)
{
  movie_t movie;
  char *csvdupe = strdup (csv);
  char *token = strtok (csvdupe, ",");
  movie.title = strdup (token); // make a copy the first token as the title

  // Use strtok() repeatedly to get each string one at a time and format
  // the data fields as follows:
  token = strtok (NULL, ",");

  //   Use strtol() on the next token as year
  movie.year = strtol (token, NULL, 10);

  //   Copy the token as the movie's genre field
  token = strtok (NULL, ",");
  movie.genre = strdup (token);

  free (csvdupe);

  return movie;
}

/* Build a dynamically allocated string from a movie_t object as follows:
   "Title [Year] - Genre"
 */
char *
merge_data (movie_t movie)
{

  // There are a few ways to merge strings in C. One approach is to start with
  // a base string on the heap and repeatedly grow and append the string. In
  // pseudocode, it might look like this:
  //
  //   str = strdup ("Hello");
  //   str = realloc (str, ...length...);     // grow
  //   strncat (str, " World", ...length...); // concatenate
  //   str = realloc (str, ...length...);     // grow
  //   strncat (str, " Again", ...length...); // concatenate
  //
  // This is the C equivalent of something like this in Java:
  //
  //   str = "Hello";
  //   str += " World";
  //   str += " Again";
  //
  // Another (generally more efficient) way to do this is to use snprintf(),
  // for formatting a string. If you are not familiar with snprintf(), you
  // should be familiar with its cousin, printf(). Consider the following
  // line of code, which prints three string variables to the screen:
  //
  //   printf ("%s %s %s\n", str1, str2, str3);
  //
  // You do the exact same thing with snprintf(), but add two variables at
  // the beginning: the destination buffer (i.e., where do you want the
  // final string in memory) and the length:
  //
  //   snprintf (buffer, length, "%s %s %s\n", str1, str2, str3);
  //
  // The original strings str1, str2, and str3 are unmodified, because their
  // contents are copied into the buffer. snprintf() also has a great built-in
  // feature that strncat() doesn't: it works with more than just strings!
  //
  //   int num = 42;
  //   char *str = "My favorite number is ";
  //   snprintf (message, length, "%s%d\n", str, num);
  //
  // Just remember that the message/buffer variable must be a pointer to a
  // writable portion of memory where you have allocated enough space to store
  // the combined string.

  // HINT: When doing string manipulation, you should use a variable to keep
  // explicit track of the string length. C string lengths are not inherently
  // stored anywhere.
  size_t len = strlen (movie.title) + strlen (movie.genre) + 10 + 1;
  // always keep track of the null byte

  // Use one of the two techniques above to allocated enough space for the
  // merged string. You should NOT rely on any length unless its exact value
  // is known. For instance, you can assume that " [Year] - " adds 10 bytes,
  // but you cannot assume anything about the length of the genre or title.
  // As such, you need to compute those lengths using strlen(). Do not forget
  // to account for the null byte.
  char *result = calloc (len, sizeof (char));

  snprintf (result, len, "%s [%d] - %s", movie.title, movie.year, movie.genre);

  return result;
}
