#include "payload.h"

static void sigchld_handler(int sig, siginfo_t *info, void *_) {
	extern pid_t client_seats[3];
	pid_t		 pid;

	if (sig != SIGCHLD) {
		return;
	}
	pid = info->si_pid;
	for (size_t i = 0; i < sizeof client_seats / sizeof *client_seats; i++) {
		if (client_seats[i] == pid) {
			client_seats[i] = -1;
			break;
		}
	}
}

/*
 * Returns 0 on success and -1 on failure
 */
int install_sigchld_handler(void) {
	struct sigaction sa = {.sa_sigaction = sigchld_handler,
						   .sa_flags = SA_RESTART | SA_NOCLDSTOP | SA_SIGINFO};
	if (sigemptyset(&sa.sa_mask) != 0) {
		return (-1);
	}
	return (sigaction(SIGCHLD, &sa, NULL));
}
