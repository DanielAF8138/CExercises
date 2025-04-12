#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "signals.h"

static void catch_loop (int);

/* Convert the short signal name (NNN part of SIGNNN) to its number.
   Consult /usr/include/asm/signal.h for the number mappings. For
   this lab, hard-code the numbers; do not use the numeric constants.
   Return -1 if the name isn't one you need to look up. */
int
signal_lookup (char *name)
{
  if (name == NULL)
    {
      return -1;
    }
  // TODO: Look up the numbers for INT, SEGV, KILL, and TSTP
  if (strcmp (name, "INT") == 0)
    {
      return 2;
    }
  else if (strcmp (name, "SEGV") == 0)
    {
      return 11;
    }
  else if (strcmp (name, "KILL") == 0)
    {
      return 9;
    }
  else if (strcmp (name, "TSTP") == 0)
    {
      return 20;
    }
  else
    {
      return -1;
    }
}

// Entry point for overriding a signal handler in this lab.
void
override (char *signal)
{
  int sig = signal_lookup (signal);
  if (sig == SIGKILL)
    printf ("Received bad signal name: %s\n", signal);
  else
    catch_loop (sig);
}

// Signal handler for SIGINT. This function doesn't use the signal number.
void
sigint_handler (int signum __attribute__ ((unused)))
{
  // TODO: Complete this signal handler so that it writes the message
  // "Breaking out with signal %d\n" with the signal number that was
  // passed. Note that you cannot use printf() in a signal handler.
  // You must use write(). After writing the message, exit with return
  // code 37.
  //
  // HINT: If you fail integration tests and get a message that the
  // binary files are different, you may be writing an extra byte (the
  // null byte). You can make sure you avoid this by using strlen()
  // instead of trying to hard-code a length.
  char out[64];
  int len = sprintf (out, "Breaking out with signal %d\n", signum);
  write (1, out, len);
  exit (37);
}

void
catch_loop (int signum)
{
  // TODO: Override the signal handler for the signal number passed.
  // Then create a child with the following code:
  //   printf ("Child has started\n");
  //   fflush (stdout);
  //   while (1) ;

  // HINT: You will NOT call sigint_handler() directly here. Instead,
  // you must use sigaction() to modify the signal's interrupt handler
  // so that sigint_handler() will be called later when the signal occurs.

  // After starting the child, the parent will call sleep (1) to wait
  // for 1 second before sending the signal to the child. The parent
  // must wait for the child to run (and exit), then print
  // "Child was stopped with status %d\n" with the child process's exit
  // status code, retrieved with waitpid(). Note that you will need to
  // call WEXITSTATUS() on the retrieved status code.
  struct sigaction sig_action;
  memset (&sig_action, 0, sizeof (sig_action));

  sig_action.sa_handler = sigint_handler;

  if (sigaction (signum, &sig_action, NULL))
    {
      printf ("Failed to setup signal handler for signa %d\n", signum);
    }

  pid_t child_pid = fork ();

  if (child_pid < 0)
    {
      exit (1);
    }

  if (child_pid == 0)
    {
      printf ("Child has started\n");
      fflush (stdout);
      while (1)
        {
          ;
        }
      exit (0);
    }
  else
    {
      int status;
      sleep (1);
      kill (child_pid, signum);
      waitpid (child_pid, &status, 0);
      printf ("Child was stopped with status %d\n", WEXITSTATUS (status));
    }
}
