#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define TH 2

int var1 = 0, var2 = 0;

pthread_mutex_t lock1;
pthread_mutex_t lock2;

void	*routine(void *ptr)
{
	if (rand() & 1) {
		pthread_mutex_lock(&lock1);
		sleep(1);
		pthread_mutex_lock(&lock2);
	} else {
		pthread_mutex_lock(&lock2);
		sleep(1);
		pthread_mutex_lock(&lock1);
	}
	var1++;
	var2++;
	pthread_mutex_unlock(&lock1);
	pthread_mutex_unlock(&lock2);
	return ptr;
}

int main()
{
	pthread_t th[TH];

	srand(time(NULL));
	pthread_mutex_init(&lock1, NULL);
	pthread_mutex_init(&lock2, NULL);
	for (int i=0; i < TH; i++) {
		if (pthread_create(&th[i], NULL, routine, NULL) != 0) {
			printf("Error\n");
			return 1;
		}
	}
	for (int i=0; i < TH; i++) {
		if (pthread_join(th[i], NULL) != 0) {
			printf("Failed to join thread\n");
			return 2;
		}
	}
	printf("%d	%d\n", var1, var2);
	pthread_mutex_destroy(&lock1);
	pthread_mutex_destroy(&lock2);
}