#include <check.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../pipe.h"

// Change this to true if you want to inspect the files produced in
// tests/inputs for debugging
#define DEBUG false

START_TEST (UNIT_result)
{
  printf ("\n======================================\n");
  printf ("UNIT_result\n");
  printf ("Gets cksum of data/f1.txt\n");

  char *result = get_result ("cksum", "data/f1.txt");
  ck_assert_str_eq (result, "3015617425 6 data/f1.txt");
}
END_TEST

void public_tests (Suite *s)
{
  TCase *tc_public = tcase_create ("Public");
  tcase_add_test (tc_public, UNIT_result);
  suite_add_tcase (s, tc_public);
}

