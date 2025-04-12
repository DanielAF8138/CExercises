#include <fcntl.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "pipe.h"

/* Creates a child process. If this fails, close the pipe and return -1.
   If the child is created, use it to run the executable program on the
   filename passed. Send the result back through the pipe. The parent process
   should just return the child's PID. Sample call:

     int fd[2];
     pipe (fd);
     pid_t child_pid = create_child (exec, fd, "foo.txt");
  */
pid_t
create_child (char *const exec, int *pipe, char *const filename)
{
  pid_t child_pid = fork ();

  if (child_pid < 0)
    {
      close (pipe[0]);
      close (pipe[1]);
      return -1;
    }
  else if (child_pid == 0)
    {
      close (pipe[0]);
      dup2 (pipe[1], STDOUT_FILENO);
      close (pipe[1]);
      execlp (exec, exec, filename, NULL);
      _exit (EXIT_FAILURE);
    }
  else
    {
      close (pipe[1]);
      return child_pid;
    }
}

/* Runs the executable program on the input filename. Start by creating a
   pipe and calling create_child() to create the child process. If that
   fails, return NULL. Otherwise, implement the parent here so that it
   reads the result of the process from the pipe and returns the result
   without a newline. The returned string must be a dynamically allocated
   copy; you can use the constant BUFFER_LENGTH for its size. Be sure to
   close the ends of the pipe at appropriate times. Sample call:

     char *sum = get_result ("cksum", "data/f1.txt");
     // sum is "3015617425 6 data/f1.txt" [without the newline]
   */
char *
get_result (char *const exec, char *const filename)
{
  int pipe_fd[2];
  if (pipe (pipe_fd) < 0)
    {
      printf ("pipe failure in get result\n");
      return NULL;
    }

  pid_t child_pid = create_child (exec, pipe_fd, filename);
  if (child_pid < 0)
    {
      printf ("create child failure in get result\n");
      return NULL;
    }

  close (pipe_fd[1]);

  char *buffer = calloc (BUFFER_LENGTH, sizeof (char));

  ssize_t bytecount = read (pipe_fd[0], buffer, BUFFER_LENGTH - 1);

  if (bytecount < 0)

    {
      free (buffer);
      close (pipe_fd[0]);
      return NULL;
    }

  buffer[bytecount] = '\0';

  char *newline = strchr (buffer, '\n');
  if (newline)
    {
      *newline = '\0';
    }

  char *output = calloc (strlen (buffer) + 1, sizeof (char));

  strncpy (output, buffer, strlen (buffer) + 1);

  close (pipe_fd[0]);
  free (buffer);
  return output;
}

/* Re-implementation of the create_child() in pipe.c. Instead of
   using the calls to pipe/fork/dup2/exec, combine the latter three
   into a call to posix_spawn(). Sample call:

     char *sum = spawn_result ("cksum", "data/f1.txt");
     // sum is "3015617425 6 data/f1.txt\n" [with the newline]
   */
char *
spawn_result (char *const exec, char *const filename)
{
  // Instead of using fork() and exec(), use the posix_spawn functions.
  // Note that you will need to create a pipe, initialize a
  // posix_spawn_file_actions_t instance, and add close and dup2 actions
  // to it so that the child process writes its STDOUT to the pipe.
  // After spawning the new process, make sure to call
  // posix_spawn_file_actions_destroy() on the actions to prevent
  // memory leaks. To avoid looking up the executable in the $PATH,
  // you can use posix_spawnp().

  // Parent code: read the value back from the pipe into a dynamically
  // allocated buffer. Wait for the child to exit, then return the
  // buffer. Make sure to close the ends of the pipe in appropriate
  // places.

  int pipe_fd[2];
  pipe (pipe_fd);

  posix_spawn_file_actions_t actions;
  posix_spawn_file_actions_init (&actions);

  posix_spawn_file_actions_addclose (&actions, pipe_fd[0]);
  posix_spawn_file_actions_adddup2 (&actions, pipe_fd[1], STDOUT_FILENO);

  pid_t child_pid;
  char *argv[] = { exec, filename, NULL };

  int status = posix_spawnp (&child_pid, exec, &actions, NULL, argv, NULL);

  posix_spawn_file_actions_destroy (&actions);

  if (status != 0)
    {
      close (pipe_fd[0]);
      close (pipe_fd[1]);
      char *nullbyte = calloc (1, sizeof (char));
      *nullbyte = '\0';
      return nullbyte;
    }

  wait (NULL);

  close (pipe_fd[1]);

  char *buffer = calloc (BUFFER_LENGTH, sizeof (char));
  ssize_t numbytesread = read (pipe_fd[0], buffer, BUFFER_LENGTH - 1);

  if (numbytesread <= 0)
    {
      free (buffer);
      close (pipe_fd[0]);

      char *nullbyte = calloc (1, sizeof (char));
      *nullbyte = '\0';
      return nullbyte;
    }

  char *newline = strchr (buffer, '\n');
  if (newline)
    {
      *newline = '\0';
    }

  return buffer;
}
