#ifndef __pipe_h__
#define __pipe_h__

#include <unistd.h>

#define BUFFER_LENGTH 100

pid_t create_child (char *const, int *, char *const);
char *get_result (char *const, char *const);
char *spawn_result (char *const, char *const);

#endif
