#include "trojan.h"
/*
 * Make the process a daemon as described in the
 * chapter 37.2 (creating a daemon) of the "The Linux Programming Interface"
 */
int daemonize(void) {
	int dev_null_fd;

	switch (fork()) {
	case (0):
		break;
	case (-1):
		return (-1);
	default:
		exit(0);
	}
	if (setsid() == -1) {
		return (-1);
	}
	/* Ensure we're not session a leader with this second fork,
	 * in case of the process acquiries a device and so the acquired device
	 * is not set as the controlling terminal
	 */
	switch (fork()) {
	case (0):
		break;
	case (-1):
		return (-1);
	default:
		exit(0);
	}
	/*
	 * change the process' mask and it's cwd to "/"
	 * to avoid blocking the system from unmounting any dirs.
	 */
	umask(0);
	if (chdir("/") != 0) {
		return (-1);
	}
	/*
	 * Close all standard io fds and redirect them to /dev/null
	 */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	dev_null_fd = open("/dev/null", O_RDWR);
	if (dev_null_fd != 0) {
		return (-1);
	}
	if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO) {
		return (-1);
	}
	if (dup2(STDOUT_FILENO, STDIN_FILENO) != STDIN_FILENO) {
		return (-1);
	}
	return (0);
}
