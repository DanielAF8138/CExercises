#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <check.h>

#include "../movies.h"

// Test that split_data gets the title copy
START_TEST (UNIT_split_title)
{
  char *csv = "Casablanca,1942,Romance";
  movie_t movie = split_data (csv);
  ck_assert_str_eq (csv, "Casablanca,1942,Romance");
  ck_assert_str_eq (movie.title, "Casablanca");
}
END_TEST

// Test that split_data parses numeric values
START_TEST (UNIT_split_nums)
{
  char *csv = "Casablanca,1942,Romance";
  movie_t movie = split_data (csv);
  ck_assert_str_eq (csv, "Casablanca,1942,Romance");
  ck_assert_int_eq (movie.year, 1942);
}
END_TEST

// Test that split_data gets the genre copy
START_TEST (UNIT_split_genre)
{
  char *csv = "Casablanca,1942,Romance";
  movie_t movie = split_data (csv);
  ck_assert_str_eq (csv, "Casablanca,1942,Romance");
  ck_assert_str_eq (movie.genre, "Romance");
}
END_TEST

// Test that split_data operates correctly on stack-based strings
START_TEST (UNIT_split_stack)
{

  char csv[] = "Casablanca,1942,Romance";
  movie_t movie = split_data (csv);
  ck_assert_str_eq (movie.title, "Casablanca");
  ck_assert_int_eq (movie.year, 1942);
  ck_assert_str_eq (movie.genre, "Romance");
  ck_assert_str_eq (csv, "Casablanca,1942,Romance");
}
END_TEST

// Test that merge_data combines the fields into a string
START_TEST (UNIT_merge_movie)
{
  movie_t movie;
  movie.title = "The Lord of the Rings: The Fellowship of the Ring";
  movie.year = 2001;
  movie.genre = "Fantasy";
  char *result = merge_data (movie);
  ck_assert_str_eq (result, "The Lord of the Rings: The Fellowship of the Ring [2001] - Fantasy");
  free (result);
}
END_TEST


// Test that merge_data combines the fields from the heap into a string
START_TEST (UNIT_merge_heap)
{
  movie_t *movie = calloc (1, sizeof (movie_t));
  movie->title = "The Lord of the Rings: The Fellowship of the Ring";
  movie->year = 2001;
  movie->genre = "Fantasy";
  char *result = merge_data (*movie);
  ck_assert_str_eq (result, "The Lord of the Rings: The Fellowship of the Ring [2001] - Fantasy");
  free (result);
  result = NULL;
  ck_assert_str_eq (movie->title, "The Lord of the Rings: The Fellowship of the Ring");
  ck_assert_int_eq (movie->year, 2001);
  ck_assert_str_eq (movie->genre, "Fantasy");
  free (movie);
}
END_TEST

void public_tests (Suite *s)
{
  TCase *tc_public = tcase_create ("Public");
  tcase_add_test (tc_public, UNIT_split_title);
  tcase_add_test (tc_public, UNIT_split_nums);
  tcase_add_test (tc_public, UNIT_split_genre);
  tcase_add_test (tc_public, UNIT_split_stack);
  tcase_add_test (tc_public, UNIT_merge_movie);
  tcase_add_test (tc_public, UNIT_merge_heap);
  suite_add_tcase (s, tc_public);
}

