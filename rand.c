#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
	srand(time(NULL));

	int count = 1000;
	if(argc == 2) {
		count = atoi(argv[1]);
	}
	fprintf(stderr, "Generating %d random numbers.\n", count);

	printf("%d \n", count);
	while(count--) {
		printf("%d ", rand());
	}

	return 0;
}
