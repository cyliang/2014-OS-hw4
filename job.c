#include "job.h"
#include <semaphore.h>

struct job job_queue[100];

int job_queue_out = 0;
int job_queue_in = 0;
sem_t job_queue_lock; // t
/* For a better implementation, use two lock for each "push" and "pop". */

sem_t job_count; // s

void job_init() {
	sem_init(&job_queue_lock, 0, 1);
	sem_init(&job_count, 0, 0);
}

void push_job(void *(*fn)(void *), void *arg) {
	sem_wait(&job_queue_lock);

	job_queue[job_queue_in].fn = fn;
	job_queue[job_queue_in].arg = arg;
	job_queue_in++;

	sem_post(&job_queue_lock);
	sem_post(&job_count);
}

struct job pop_job() {
	sem_wait(&job_count);
	sem_wait(&job_queue_lock);

	struct job return_job = job_queue[job_queue_out++];

	sem_post(&job_queue_lock);
	return return_job;
}
