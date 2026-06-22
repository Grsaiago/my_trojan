#include "payload.h"
#include <string.h>
#include <systemd/sd-journal.h>
#include <unistd.h>

int main(void) {
	int		   listener;
	int		   client_fd;
	pid_t	   pid;
	pid_t	   main_pid = getpid();
	const char password[] = "xXgsaiagoXx";

	listener = create_listener();
	if (listener < 0) {
		NotifyStoppingf(
			main_pid, "Failed to create listener: %s", strerror(errno));
		return (1);
	}
	if (install_sigchld_handler() != 0) {
		NotifyStoppingf(
			main_pid, "Failed to install sigchld handler: %s", strerror(errno));
		close(listener);
		return (1);
	}
	while (42) {
		client_fd = accept(listener, NULL, NULL);
		if (client_fd < 0) {
			Err("Failed to accept new client");
			NotifyStoppingf(
				main_pid, "Failed to accept a new client: %s", strerror(errno));
			close(listener);
			return (1);
		}
		if (client_buffer_has_seat() != 0) {
			Debug("The buffer has no seats available, refusing new client");
			close(client_fd);
			continue;
		}

		pid = fork();
		if (pid < 0) {
			Err("Failed to fork process to handle new client");
			write(client_fd, "failed to handle your connection\n", 34);
			close(client_fd);
			continue;
		}
		if (pid == 0) {
			if (handle_client(password, client_fd) != 0) {
				close(client_fd);
				return (1);
			}
		}
		client_buffer_acquire_seat(pid);
		close(client_fd); // avoid leak on parent process
	}
}
