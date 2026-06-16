#ifndef __PAYLOAD_INC
#define __PAYLOAD_INC

#include "shared.h"

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <systemd/sd-daemon.h>
#include <unistd.h>

extern pid_t client_seats[3];

int	 create_listener(void);
void setup_sigchld(void);

#endif // __PAYLOAD_INC
