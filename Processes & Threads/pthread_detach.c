#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define TH 3

void	*routine(void *ptr)
{
	sleep(1);
	printf("heyy\n");
	return ptr;
}

int main()
{
	pthread_t th[TH];

	for (int i=0; i < TH; i++) {
		if (pthread_create(&th[i], NULL, routine, NULL) != 0) {
			printf("Error\n");
			return 1;
		}
		pthread_detach(th[i]);
	}
	for (int i=0; i < TH; i++) {
		if (pthread_join(th[i], NULL) != 0) {
			printf("Failed to join thread\n");
			// return 2;
		}
	}
}