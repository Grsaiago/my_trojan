#include "payload.h"

int create_listener(void) {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		Err("Failed to open socket()");
		return (-1);
	}
	int optval = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(4242),
		.sin_addr = {htonl(INADDR_ANY)},
	};
	if (bind(fd, (struct sockaddr *)&addr, sizeof addr) < 0) {
		Err("Failed to bind() on socket");
		return (-1);
	}
	if (listen(fd, 3) < 0) {
		Err("Failed to listen() on socket");
		return -1;
	}
	return fd;
}
