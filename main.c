#include "header.h"
#include "pthread.h"
#include "semaphore.h"

struct thread_arg {
	int height;
	sem_t start;
	sem_t *finish;

	int head, tail;
};

void *par_thread(void *args);

int ary[1000000];

int main(int argc, char *argv[]) {
	if(argc != 2) {
		fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
		exit(1);
	}

	int count;
	count = read_file(argv[1], ary);

	pthread_t init_thread;
	struct thread_arg *init_arg = (struct thread_arg *) malloc(sizeof(struct thread_arg));

	sem_t finish;
	sem_init(&init_arg->start, 0, 0);
	sem_init(&finish, 0, 0);

	init_arg->height = 3;
	init_arg->finish = &finish;
	init_arg->head = 0, init_arg->tail = count - 1;

	pthread_create(&init_thread, NULL, par_thread, init_arg);

	int i = 3;
	while(i--) {
		printf("%d...\n", i + 1);
		sleep(1);
	}
	puts("Go!");

	sem_post(&init_arg->start);
	i = 8;
	while(i--) {
		sem_wait(&finish);
	}

	puts("Done!");
	write_file("output.txt", ary, count);
	return 0;
}

void *par_thread(void *a) {
#ifdef VERBOSE
	puts("Thread created."); fflush(stdout);
#endif
	struct thread_arg *args = (struct thread_arg *) a;

	if(args->height) {
		pthread_t pass_thread[2];
		struct thread_arg *pass_arg[2] = {
			(struct thread_arg *) malloc(sizeof(struct thread_arg)),
			(struct thread_arg *) malloc(sizeof(struct thread_arg))
		};

		sem_init(&pass_arg[0]->start, 0, 0);
		sem_init(&pass_arg[1]->start, 0, 0);

		pass_arg[0]->height = pass_arg[1]->height = args->height - 1;
		pass_arg[0]->finish = pass_arg[1]->finish = args->finish;

		pthread_create(&pass_thread[0], NULL, par_thread, pass_arg[0]);
		pthread_create(&pass_thread[1], NULL, par_thread, pass_arg[1]);

		sem_wait(&args->start);
#ifdef VERBOSE
		puts("Thread running."); fflush(stdout);
#endif

		int p = args->head, r = args->tail;
		int pivot = ary[r];
		while(p < r) {
			while(ary[p] < pivot) {
				p++;
			}
			while(ary[r] > pivot) {
				r--;
			}
			if(ary[p] == ary[r])
				p++;
			else if(p < r) {
				int tmp = ary[p];
				ary[p] = ary[r];
				ary[r] = tmp;
			}
		}

		pass_arg[0]->head = args->head, pass_arg[0]->tail = r - 1;
		pass_arg[1]->head = r + 1, pass_arg[1]->tail = args->tail;
		sem_post(&pass_arg[0]->start);
		sem_post(&pass_arg[1]->start);
	} else {
		sem_wait(&args->start);
#ifdef VERBOSE
		puts("Thread running."); fflush(stdout);
#endif

		bsort(ary + args->head, args->tail - args->head + 1);

		sem_post(args->finish);
	}

	free(args);
#ifdef VERBOSE
	puts("Thread terminated."); fflush(stdout);
#endif
}
