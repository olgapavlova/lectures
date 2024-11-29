#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char ** argv) {
	if (argc == 0) exit(EXIT_FAILURE);

	int fd_stdin = fileno(stdin);
	int fd_stdout = fileno(stdout);
	int fd_stderr = fileno(stderr);

	int fd_random = open("/dev/random", O_RDONLY, 0);
	int fd_data = open(argv[1], O_RDONLY, 0);

	printf("%d\n", fd_stdin);
	printf("%d\n", fd_stdout);
	printf("%d\n", fd_stderr);
	printf("%d\n", fd_random);
	printf("%d\n", fd_data);

	if (fd_data == -1) exit(EXIT_FAILURE);

	// read

#define BUFFER_SIZE 1024
	
	char * buffer = malloc(BUFFER_SIZE + 1);
	ssize_t received;
	ssize_t sended;
	while((received = read(fd_data, buffer, BUFFER_SIZE)) > 0) {
		sended = write(fd_stdout, buffer, received);
		if (sended != received) exit(EXIT_FAILURE);
		//sended = write(fd_stderr, buffer, received);
		//if (sended != received) exit(EXIT_FAILURE);
	}

	close(fd_random);
	close(fd_data);

	exit(EXIT_SUCCESS);
}
