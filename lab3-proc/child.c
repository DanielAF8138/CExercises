#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "child.h"

pid_t
run_child (int rc)
{
  char *str = "I am the parent";

  // TODO: Add code here to create a child process that prints
  // "I am the child" and exits with return code passed as the rc
  // parameter to this function. The parent must wait for the child
  // to finish before printing the child's exit status (consult the
  // textbook for this as needed) before printing "I am the parent".
  // The parent should return the child's PID from this function.

  pid_t child_pid = fork ();
  if (child_pid == 0)
    {
      printf ("I am the child\n");
      exit (rc);
    }

  printf ("Child exited with status %d\n", rc);

  wait (NULL);

  fprintf (stderr, "str [at %p]: %p\n", (void *)&str, (void *)str);
  printf ("%s\n", str);
  return 0;
}
