struct job {
	void *(*fn)(void *);
	void *arg;
};

void job_init();
void push_job(void *(*fn)(void *), void *arg);
struct job pop_job();
