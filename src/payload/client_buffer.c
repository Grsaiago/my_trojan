#include "payload.h"

static pid_t client_buffer[3] = {-1, -1, -1};

/*
 * Returns the available position on success and -1 on error
 */
int32_t client_buffer_has_seat(void) {
	for (uint16_t seat = 0;
		 seat < sizeof(client_buffer) / sizeof(*client_buffer); seat++) {
		if (client_buffer[seat] == -1) {
			return (seat);
		}
	}
	return (-1);
}

/*
 * Returns 0 on success and -1 on error
 */
int32_t client_buffer_acquire_seat(pid_t client_id) {
	int32_t position = client_buffer_has_seat();
	if (position < 0) {
		return (-1);
	}
	client_buffer[position] = client_id;
	return (0);
}

int32_t client_buffer_remove_seat(pid_t client_id) {
	pid_t found_client;
	for (size_t i = 0; i < sizeof client_buffer / sizeof *client_buffer; i++) {
		if (client_buffer[i] == client_id) {
			found_client = client_buffer[i];
			client_buffer[i] = -1;
			return (found_client);
		}
	}
	return (0);
}
