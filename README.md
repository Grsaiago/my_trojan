# my_trojan
A study trojan that gives the attacker a root bind shell


## Reading
systemd blog post:
https://0pointer.de/blog/projects/systemd.html

systemd docs:
https://systemd.io/

## Project strategy

Create a new-style daemon following this reference:
https://www.freedesktop.org/software/systemd/man/latest/daemon.html#

The daemon will log following this reference:
https://www.freedesktop.org/software/systemd/man/latest/sd-daemon.html#

The main executable will write a service script following this reference:
https://www.freedesktop.org/software/systemd/man/latest/systemd.service.html


We'll use self pipe method with SIGCHLD + sigaction_t to get which client disconnected;
