#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/time.h>

#define TH 16

typedef struct timeval t_timeval;

sem_t	*sem;
t_timeval timeval, timestart;

void	*routine(void *args)
{
	sem_wait(sem);
	gettimeofday(&timeval, NULL);
	printf("Player #%d is in, at %lus\n", *(int *)args, timeval.tv_sec - timestart.tv_sec);
	sleep(2);
	gettimeofday(&timeval, NULL);
	printf("Player #%d is out, at %lus\n", *(int *)args, timeval.tv_sec - timestart.tv_sec);
	sem_post(sem);
	free(args);
	return NULL;
}

int main()
{
	pthread_t th[TH];

	gettimeofday(&timestart, NULL);
	sem = sem_open("/ser", O_CREAT, 0777, 10);
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