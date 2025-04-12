#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <check.h>

#include "../effects.h"
#include "../model.h"

/* nothing but starting a new cmdline */
START_TEST (UNIT_new_cmdline)
{
  printf ("\n======================================\n");
  printf ("UNIT TEST: new_cmdline\n\n");

  fsm_t *fsm = cmdline_init ();
  assert (fsm != NULL);

  handle_event (fsm, TOKEN);
  ck_assert_int_eq (fsm->state, Command);

  free (fsm);
}
END_TEST

/* a single cmdline from NEW to TRM */
START_TEST (UNIT_exec_cmd)
{
  printf ("\n======================================\n");
  printf ("UNIT TEST: exec_cmd\n\n");

  fsm_t *fsm = cmdline_init ();
  assert (fsm != NULL);

  handle_event (fsm, TOKEN);
  ck_assert_int_eq (fsm->state, Command);

  handle_event (fsm, NEWLINE);
  ck_assert_int_eq (fsm->state, Term);

  free (fsm);
}
END_TEST

void public_tests (Suite *s)
{
  TCase *tc_public = tcase_create ("Public");
  tcase_add_test (tc_public, UNIT_new_cmdline);
  tcase_add_test (tc_public, UNIT_exec_cmd);
  suite_add_tcase (s, tc_public);
}

