#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define BUFFER_SIZE 4096

int main(int argc, char** argv) {
  int desc = open(argv[1], O_RDONLY);
  if ((desc < 0) || (argc == 1)) exit(EXIT_FAILURE);

  char* buffer = malloc(BUFFER_SIZE + 1);
  ssize_t received;
  ssize_t sended;

  while ((received = read(desc, buffer, BUFFER_SIZE)) > 0) {
	sended = write(1, buffer, received);
    if (received != sended) exit(EXIT_FAILURE);
  }

  free(buffer);
  close(desc);

  exit(EXIT_SUCCESS);
}
