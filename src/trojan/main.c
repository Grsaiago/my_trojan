#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void) {
	extern unsigned char payload[];
	extern unsigned int  payload_len;

	printf("gsaiago\n");
	int fd = open("/tmp/my_shield", O_CREAT | O_TRUNC | O_WRONLY, S_IWUSR);
	if (fd < 0) {
		printf("deu merda no open\n");
		return 0;
	}
	if (write(fd, payload, payload_len) < 0) {
		printf("write error for fd %d: %s\n", fd, strerror(errno));
	}
}
