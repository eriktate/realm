#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_file(const char *fname) {
	FILE *fd = fopen(fname, "r");
	if (fd == NULL) {
		return NULL;
	}

	// find file size
	fseek(fd, 0L, SEEK_END);
	size_t fsize = ftell(fd);
	rewind(fd);

	char *buf = (char *)malloc(fsize);

	fread(buf, 1, fsize, fd);

	return buf;
}
