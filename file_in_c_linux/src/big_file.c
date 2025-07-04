#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char ** argv) {
	FILE * f = fopen(argv[1], "w+t");
	int rand = open("/dev/random", O_RDONLY, 0);

	if ((f == NULL) || (argc == 1)) exit(EXIT_FAILURE);

	char buffer = '\0';
	for (unsigned long i = 0ul; i < 4294967295ul; ++i) {
		read(rand, &buffer, sizeof(char));
		char c = (buffer < 0) ? -buffer : buffer;
		fprintf(f, "%c", c); 
	}

	close(rand);
	fclose(f);
	exit(EXIT_SUCCESS);
}
