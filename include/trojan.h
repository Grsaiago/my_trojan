#ifndef __TROJAN_INC
#define __TROJAN_INC

#include "shared.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

int patch_payload(unsigned char *payload, unsigned int payload_len);

void start_daemon(void);

#endif // __TROJAN_INC
