#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "error_functions.h"
#include "unix_sockets.h"

#define BUF_SIZE 500

int
main(int argc, char **argv)
{
	const char const *sv_sock_path = "/tmp/wind_sv";

	struct sockaddr_un addr;
	ssize_t num_read;
	char buf[BUF_SIZE];

	int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	printf("Client socket fd = %d\n", sfd);

	if (sfd == -1) {
		err_exit("socket");
	}

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, sv_sock_path, sizeof(addr.sun_path) - 1);

	if (connect(sfd, (struct sockaddr *) &addr,
			sizeof(struct sockaddr_un)) == -1) {
		err_exit("connect");
	}

	while ((num_read = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
		if (write(sfd, buf, num_read) != num_read) {
			fatal("partial/failed write");
		}
	}

	if (num_read == -1) {
		err_exit("read");
	}

	/* Closes our socket; server sees EOF. */
	exit(EXIT_SUCCESS);
}
