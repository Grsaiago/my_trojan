#include "payload.h"

pid_t client_seats[3] = {-1, -1, -1};

/*
 * Returns 0 on success and -1 on error
 */
int client_buffer_has_seat(void) {
	for (size_t seat = 0; seat < 3; seat++) {
		if (client_seats[seat] == -1) {
			return (0);
		}
	}
	return (-1);
}

/*
 * Returns 0 on success and -1 on error
 */
int client_buffer_acquire_seat(pid_t new_seat) {
	for (size_t seat = 0; seat < (sizeof(client_seats) / sizeof(*client_seats));
		 seat++) {
		if (client_seats[seat] == -1) {
			client_seats[seat] = new_seat;
			return (0);
		}
	}
	return (-1);
}
