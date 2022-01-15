#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void	*routine(void *ptr)
{
	printf("Start of thread\n");
	sleep(1);
	printf("End of thread\n");
	return ptr;
}

int main(int ac, char **av)
{
	pthread_t th[4];
	
	for (int i=0; i < 4; i++) {
		if (pthread_create(&th[i], NULL, routine, NULL) != 0) {
			printf("Error\n");
			return 1;
		}
	}
	
	for (int i=0; i < 4; i++) {
		if (pthread_join(th[i], NULL) != 0) {
			printf("Error\n");
			return 2;
		}
	}
}