#include "payload.h"

static void sigchld_handler(int sig, siginfo_t *info, void *_) {
	extern pid_t client_seats[3];

	if (sig != SIGCHLD) {
		return;
	}
	client_buffer_remove_seat(info->si_pid);
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
	if (sigaction(SIGCHLD, &sa, NULL) != 0) {
		Err("Failed accept new client");
		return (-1);
	}
	return (0);
}
