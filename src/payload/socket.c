#include "payload.h"

int create_listener(void) {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		dprintf(STDERR_FILENO,
				SD_ERR "[file: %s][line %d][func %s] Failed to open socket()",
				__FILE_NAME__, __LINE__, __PRETTY_FUNCTION__);
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
		dprintf(STDERR_FILENO,
				SD_ERR
				"[file: %s][line %d][func %s] Failed to bind() on socket",
				__FILE_NAME__, __LINE__, __PRETTY_FUNCTION__);
		return (-1);
	}
	if (listen(fd, 3) < 0) {
		dprintf(STDERR_FILENO,
				SD_ERR
				"[file: %s][line %d][func %s] Failed to listen() on socket",
				__FILE_NAME__, __LINE__, __PRETTY_FUNCTION__);
		return -1;
	}
	return fd;
}
