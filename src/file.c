#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *fname) {
	char buf[512];
	FILE *fd = fopen(fname, "r");

	fgets(buf, 512, fd);
	printf("%s", buf);

	return (char *)malloc(512);
}
