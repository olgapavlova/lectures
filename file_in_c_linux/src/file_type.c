#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char ** argv) {
	if (argc < 2) exit(EXIT_FAILURE);
	char * filename = argv[1];

	struct stat fs;

	stat(filename, &fs);

	if ((fs.st_mode & S_IFMT) == S_IFDIR) {
		printf("Directory\n");
	} else {
		printf("Size: %lld\n", fs.st_size);
	}

	printf("%d\n", S_ISDIR(fs.st_mode));
}
