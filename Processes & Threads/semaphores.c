#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#define TH 4

sem_t	*sem;

#include <limits.h>
void	*routine(void *args)
{
	sem_wait(sem);
	sleep(1);
	// printf("Start of thread %d\n", *(int *)args);
	printf("End of thread %d\n", *(int *)args);
	sem_post(sem);
	free(args);
	return NULL;
}

int main()
{
	pthread_t th[TH];

	sem = sem_open("/sm", O_CREAT, 0777, 3);
	for (int i=0; i < TH; i++) {
		int *a = malloc(sizeof(int));
		*a = i;
		if (pthread_create(&th[i], NULL, routine, a) != 0) {
			printf("Failed to create thread %d\n", i);
			return 1;
		}
	}
	for (int i=0; i < TH; i++) {
		if (pthread_join(th[i], NULL) != 0) {
			printf("Failed to join thread %d\n", i);
		}
	}
	sem_close(sem);
}