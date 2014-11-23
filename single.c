#include "header.h"

int main(int argc, char *argv[]) {
	if(argc != 2) {
		fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
		exit(1);
	}

	int ary[1000000];
	int count;
	count = read_file(argv[1], ary);

	bsort(ary, count);
	write_file("output.txt", ary, count);

	return 0;
}
