2014-OS-hw4
===========

This repositary does not follow all of the requirements of this assignment.

It does not prompt for input file and thread count, and does not read anything from *stdin*. 
Instead, it reads *command line arguments*.

What are the points?
--------------------
All the implementation of the job queue(job list) are in `job.c` and `job.h`.

Programmer uses `push_job(job_function, job_argument)` to push a new job into the queue.

The worker `thread_worker()` in `main.c` uses `pop_job()` to get a job popped from the queue and works for that job.

To compile
----------
``` sh
make
```

To execute
----------
``` sh
./MT <input_file> <thread_count>
```

To generate random numbers
--------------------------
``` sh
make rand
./rand <number_of_random_numbers> > input.txt
```
Where `input.txt` has those output random numbers.
