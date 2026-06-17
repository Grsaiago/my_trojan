#ifndef __PAYLOAD_INC
#define __PAYLOAD_INC

#include "shared.h"

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <systemd/sd-daemon.h>
#include <unistd.h>

#define Debug(msg)                                                             \
	(dprintf(STDOUT_FILENO,                                                    \
			 SD_DEBUG "[file: %s][line: %d][func: %s] " msg "\n",              \
			 __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__))

#define Info(msg)                                                              \
	(dprintf(STDOUT_FILENO,                                                    \
			 SD_INFO "[file: %s][line: %d][func: %s] " msg "\n",               \
			 __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__))

#define Warn(msg)                                                              \
	(dprintf(STDERR_FILENO,                                                    \
			 SD_WARNING "[file: %s][line: %d][func: %s] " msg "\n",            \
			 __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__))

#define Err(msg)                                                               \
	(dprintf(STDERR_FILENO, SD_ERR "[file: %s][line: %d][func: %s] " msg "\n", \
			 __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__))

extern pid_t client_seats[3];

int install_sigchld_handler(void);
int create_listener(void);

int client_buffer_has_seat(void);
int client_buffer_acquire_seat(pid_t new_seat);

ssize_t read_with_timeout(int fd, char *buffer, size_t max_bytes,
						  int timeout_seconds);
#endif // __PAYLOAD_INC
