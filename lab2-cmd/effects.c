#define _POSIX_C_SOURCE 200809L // needed for strdup extension

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "effects.h"
#include "model.h"

/* Executed when starting to process a new command line. The fsm_t
   should have been updated to include a pointer to the current token.
   For instance, if the command line was "ls -l data NL", the fsm_t
   has a field that points to "ls". */
void
start_command (fsm_t *cmdmodel)
{
  if (cmdmodel->current_token == NULL)
    return;
  printf ("Starting new command: %s\n", cmdmodel->current_token);
  // TODO: Copy the current token to store it in the FSM's command
  // field. Next, create the FSM's args array (length MAX_ARGUMENTS)
  // set the current token as args[0], and initialize nargs to be
  // the number of arguments (1 at this point).

  // if (cmdmodel->command != NULL)
  //   {
  //     free (cmdmodel->command);
  //     cmdmodel->command = NULL;
  //   }
  cmdmodel->command = cmdmodel->current_token;

  // if (cmdmodel->args != NULL)
  //   {
  //     free (cmdmodel->args);
  //     cmdmodel->args = NULL;
  //   }
  cmdmodel->args = calloc (MAX_ARGUMENTS, sizeof (char *));

  // if (cmdmodel->args == NULL)
  //   {
  //     return;
  //   }

  cmdmodel->args[0] = cmdmodel->current_token;
  cmdmodel->nargs = 1;
}

/* Executed when processing a token after the command name. For instance,
   if the command line was "ls -l data NL", this function will be called
   when the current token is "-l" and again when it is "data". */
void
append (fsm_t *cmdmodel)
{
  if (cmdmodel->nargs >= MAX_ARGUMENTS)
    return;

  printf ("Appending %s to the argument list\n", cmdmodel->current_token);
  assert (cmdmodel->args != NULL);

  // TODO: Store the current token into the args array and increment nargs
  cmdmodel->args[cmdmodel->nargs] = strdup (cmdmodel->current_token);
  if (!cmdmodel->args[cmdmodel->nargs])
    {
      exit (EXIT_FAILURE);
    }
  cmdmodel->nargs++;
}

/* Executed when either a NL or | (pipe) is encountered. For instance, if
   the command line is "ls -l data NL", the current token will be "NL"; also,
   the FSM's args array should be complete, containing "ls", "-l", and "data",
   followed by several NULL pointers. */
void
execute (fsm_t *cmdmodel)
{
  if (cmdmodel->args == NULL)
    return;

  // TODO: Print out the argument list similar to the format shown and free
  // the args array.

  printf ("Execute %s with arguments { ", cmdmodel->command);
  for (int i = 0; i < cmdmodel->nargs; i++)
    {
      if (cmdmodel->args[i] != NULL)
        {
          printf ("%s, ", cmdmodel->args[i]);
        }
    }
  printf ("(null) }\n");

  //   free (cmdmodel->command);
  //   for (int i = 0; i < cmdmodel->nargs; i++)
  //     {
  //       free (cmdmodel->args[i]);
  //     }

  //   free (cmdmodel->args);

  //   cmdmodel->args = NULL;
}

// No changes are needed to the effects below

void
link_commands (fsm_t *cmdmodel)
{
  printf ("Set up pipe\n");
  execute (cmdmodel);
}

void
error_pipe (fsm_t *cmdmodel)
{
  printf ("ERROR: Received token %s while in state %s\n",
          cmdmodel->current_token, state_name (cmdmodel->state));
}

void
error_newline (fsm_t *cmdmodel)
{
  printf ("ERROR: Received token %s while in state %s\n",
          cmdmodel->current_token, state_name (cmdmodel->state));
}
