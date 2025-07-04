#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {

	FILE * f = fopen(argv[1], "r");
	char * line = NULL;
	size_t buffer = 0;
	ssize_t len;

	if ((f == NULL) || (argc == 1)) exit(EXIT_FAILURE);
	
	int i = 0;
	while ((len = getline(&line, &buffer, f)) != -1)
		printf("%4d [ %2zu • %2zu ] %s\n", i++, len, buffer, line);

	free(line); fclose(f);
	exit(EXIT_SUCCESS);

	//return 0;
}
