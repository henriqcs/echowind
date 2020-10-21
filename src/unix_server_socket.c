#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "unix_sockets.h"
#include "error_functions.h"

#define BACKLOG 5
#define BUF_SIZE 80

void
main(void)
{
	const char const *sv_sock_path = "/tmp/wind_sv"; /* server socket file */

	int sfd, cfd; /* socket file descriptors */
	struct sockaddr_un addr;

	ssize_t num_read;
	char buf[BUF_SIZE];

	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	printf("Server socket fd = %d\n", sfd);

	if (sfd == -1) {
		err_exit("socket");
	}

	if (strlen(sv_sock_path) > sizeof(addr.sun_path) - 1) {
		fatal("Server socket path too long: %s", sv_sock_path);
	}

	if (remove(sv_sock_path) == -1 && errno != ENOENT) {
		err_exit("remove-%s", sv_sock_path);
	}

	memset(&addr, '\0', sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, sv_sock_path, sizeof(addr.sun_path) - 1);

	if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
		err_exit("bind");
	}

	if (listen(sfd, BACKLOG) == -1) {
		err_exit("listen");
	}

	while (1) { /* Handle client connections iteratively */
		printf("Waiting to accept a connection...\n");
		cfd = accept(sfd, NULL, NULL);
		printf("Accepted socket fd = %d\n", cfd);

		while ((num_read = read(cfd, buf, BUF_SIZE)) > 0) {
			if (write(STDOUT_FILENO, buf, num_read) != num_read) {
				fatal("partial/failed write");
			}
		}

		if (num_read == -1) {
			err_exit("read");
		}

		if (close(cfd) == -1) {
			err_msg("close");
		}
	}
}
