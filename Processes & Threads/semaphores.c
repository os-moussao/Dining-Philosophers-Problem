#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define TH 5

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

	if (sem_unlink("/semm") == -1)
	{
		perror("Error 1");
		return 1;
	}
	// unsigned int perm = S_IRWXU | S_IRWXG | S_IRWXO;
	sem = sem_open("/semm", O_CREAT | O_EXCL, S_IRWXU, 2);
	if (sem == SEM_FAILED)
	{
		perror("Error 2");
		return 1;
	}
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