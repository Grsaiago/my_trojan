#include "payload.h"

ssize_t read_with_timeout(int fd, char *buffer, size_t max_bytes,
						  int timeout_seconds) {
	fd_set		   read_fds;
	struct timeval timeout;

	FD_ZERO(&read_fds);
	FD_SET(fd, &read_fds);

	timeout.tv_sec = timeout_seconds;
	timeout.tv_usec = 0;

	int activity = select(fd + 1, &read_fds, NULL, NULL, &timeout);

	if (activity < 0) {
		return -1;
	} else if (activity == 0) {
		return 0;
	}

	if (FD_ISSET(fd, &read_fds)) {
		return read(fd, buffer, max_bytes);
	}

	return -1;
}
