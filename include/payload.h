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
#include <sys/syslog.h>
#include <sys/wait.h>
#include <systemd/sd-daemon.h>
#include <systemd/sd-journal.h>
#include <threads.h>
#include <unistd.h>

// macros as per:
// https://www.freedesktop.org/software/systemd/man/latest/sd_journal_print.html#
// https://www.freedesktop.org/software/systemd/man/latest/systemd.journal-fields.html#
#define Debug(msg)                                                             \
	(sd_journal_send("MESSAGE=%s", msg, "PRIORITY=%i", LOG_DEBUG, "TID=%lu",   \
					 thrd_current(), NULL))

#define Info(msg)                                                              \
	(sd_journal_send("MESSAGE=%s", msg, "PRIORITY=%i", LOG_INFO, "TID=%lu",    \
					 thrd_current(), NULL))

#define Warn(msg)                                                              \
	(sd_journal_send("MESSAGE=%s", msg, "PRIORITY=%i", LOG_WARNING,            \
					 "ERRNO=%d", errno, "TID=%lu", thrd_current(), NULL))

#define Err(msg)                                                               \
	(sd_journal_send("MESSAGE=%s", msg, "PRIORITY=%i", LOG_ERR, "ERRNO=%d",    \
					 errno, "TID=%lu", thrd_current(), NULL))

int install_sigchld_handler(void);
int create_listener(void);

int		client_buffer_has_seat(void);
int		client_buffer_acquire_seat(pid_t client_id);
int32_t client_buffer_remove_seat(pid_t client_id);

ssize_t read_with_timeout(int fd, char *buffer, size_t max_bytes,
						  int timeout_seconds);
#endif // __PAYLOAD_INC
