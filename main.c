#include "header.h"
#include "job.h"
#include <pthread.h>
#include <semaphore.h>

void *worker_thread(void *a) {
	while(1) {
		struct job work_job = pop_job();
		work_job.fn(work_job.arg);
	}
}

struct sort_range {
	int head;
	int tail;
};
struct partition_arg {
	int height;
	sem_t *finish;
	struct sort_range range;
};
struct sort_arg {
	sem_t *finish;
	struct sort_range range;
};

void *partition(void *args);
void *sort(void *args);

int ary[1000000];

int main(int argc, char *argv[]) {
	if(argc != 3) {
		fprintf(stderr, "Usage: %s <input_file> <thread_count>\n", argv[0]);
		exit(1);
	}

	int i;
	job_init();

	int thread_count = atoi(argv[2]);
	pthread_t thread_pool[thread_count];
	for(i=0; i<thread_count; i++)
		pthread_create(&thread_pool[i], NULL, worker_thread, NULL);

	int count;
	count = read_file(argv[1], ary);

	sem_t finish;
	sem_init(&finish, 0, 0);

	struct partition_arg *init_arg = (struct partition_arg *) malloc(sizeof(struct partition_arg));
	init_arg->height = 3;
	init_arg->finish = &finish;
	init_arg->range.head = 0, init_arg->range.tail = count - 1;

	push_job(partition, init_arg);

	i = 8;
	while(i--) {
		sem_wait(&finish);
	}

	write_file("output.txt", ary, count);
	return 0;
}

void *partition(void *a) {
	struct partition_arg *args = (struct partition_arg *) a;

	int p = args->range.head, r = args->range.tail;
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

	struct sort_range range[2];
	range[0].head = args->range.head, range[0].tail = r - 1;
	range[1].head = r + 1, range[1].tail = args->range.tail;

	if(--args->height) {
		struct partition_arg *pass_arg[2] = {
			(struct partition_arg *) malloc(sizeof(struct partition_arg)),
			(struct partition_arg *) malloc(sizeof(struct partition_arg))
		};

		pass_arg[0]->finish = pass_arg[1]->finish = args->finish;
		pass_arg[0]->height = pass_arg[1]->height = args->height;
		pass_arg[0]->range = range[0];
		pass_arg[1]->range = range[1];

		push_job(partition, pass_arg[0]);
		push_job(partition, pass_arg[1]);
	} else {
		struct sort_arg *pass_arg[2] = {
			(struct sort_arg *) malloc(sizeof(struct sort_arg)),
			(struct sort_arg *) malloc(sizeof(struct sort_arg))
		};

		pass_arg[0]->finish = pass_arg[1]->finish = args->finish;
		pass_arg[0]->range = range[0];
		pass_arg[1]->range = range[1];

		push_job(sort, pass_arg[0]);
		push_job(sort, pass_arg[1]);
	}

	free(args);
}

void *sort(void *a) {
	struct sort_arg *args = (struct sort_arg *) a;
	bsort(ary + args->range.head, args->range.tail - args->range.head + 1);
	sem_post(args->finish);
	free(args);
}
