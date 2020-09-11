#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define PAGE_SIZE		(4096)
#define NR_THREADS 		(8)
#define MB_PER_THREAD 		(2000)
#define PAGES_PER_THREAD	(MB_PER_THREAD * 256)
#define BYTES_PER_THREAD	(PAGES_PER_THREAD * PAGE_SIZE)
#define ITERATION_COUNT		(1)

void* alloc_and_die(void *data)
{
	int itr;
	char *ptr, *limit;

	for (itr = 0; itr < 3000; itr++) {
		/* Allocate 8MB in each iteration. */
		/* This should result in process termination via oom. */
		ptr = malloc(PAGE_SIZE * 256 * 8);
		if (!ptr) {
			printf("Could not allocate memory !!!\n");
			exit(1);
		}

		limit = ptr + PAGE_SIZE * 256 * 8;

		while(ptr < limit) {
			*ptr = 'A';
			ptr += PAGE_SIZE;
		}
	}
}

int main(int argc, char *argv[])
{
	int i, arg;
	pthread_t pthread[NR_THREADS];
	pthread_t pthread_die[NR_THREADS];

	arg = atoi(argv[1]);

	/* For 1, allocate and randomize free lists */
	if (arg != 1) {
		for (i=0; i < NR_THREADS; i++) 
			pthread_create(&pthread[i], NULL, alloc_and_die, NULL);
	
		for (i=0; i < NR_THREADS; i++)
			pthread_join(pthread[i],NULL);
	}
	/* Alloc and die for all other arguments */
	else {
		//alloc_and_die();
	}
	return 0;
}
