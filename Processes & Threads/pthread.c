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
	pthread_t	t1, t2;
	
	if (pthread_create(&t1, NULL, routine, NULL) != 0) {
		printf("Error\n");
		return 1;
	}
	if (pthread_create(&t2, NULL, routine, NULL) != 0) {
		printf("Error\n");
		return 1;
	}
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}