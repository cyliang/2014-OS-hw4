#include "header.h"

int read_file(const char *name, int *buf) {
	FILE *f = fopen(name, "r");
	if(!f) {
		fprintf(stderr, "Cannot open file: %s\n", name);
		exit(1);
	}

	int i = 0;
	while(!feof(f)) {
		fscanf(f, "%d", buf + i++);
	}

	fclose(f);
	return i;
}

void write_file(const char *name, int *buf, int count) {
	FILE *f = fopen(name, "w");
	if(!f) {
		fprintf(stderr, "Cannot open file: %s\n", name);
		exit(1);
	}

	int i;
	for(i=0; i<count; i++) {
		fprintf(f, "%d ", buf[i]);
	}

	fclose(f);
}

void bsort(int *buf, int count) {
	int swapped;
	do {
		swapped = 0;
		
		int i;
		for(i=1; i<count; i++) {
			if(buf[i-1] > buf[i]) {
				int temp = buf[i-1];
				buf[i-1] = buf[i];
				buf[i] = temp;

				swapped = 1;
			}
		}
	} while(swapped);
}
