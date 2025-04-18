#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>

#include <check.h>

extern void public_tests (Suite *s);
void run_testsuite (void);

Suite *
test_suite (void)
{
  Suite *s = suite_create ("Default");
  public_tests (s);
  return s;
}

void
run_testsuite (void)
{
  Suite *s = test_suite ();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  srunner_free (sr);
}

int
main (void)
{
  srand((unsigned)time(NULL));
  run_testsuite ();
  return EXIT_SUCCESS;
}
