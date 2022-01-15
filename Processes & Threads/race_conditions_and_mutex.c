#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int	n = 0;

pthread_mutex_t lock;

void	*routine(void *ptr)
{
	for (int i = 0; i < 10000000; i++)
	{
		pthread_mutex_lock(&lock);
		n++;
		pthread_mutex_unlock(&lock);
	}
	return ptr;
}

int main(int ac, char **av)
{
	pthread_t	t1, t2;
	
	pthread_mutex_init(&lock, NULL);
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
	pthread_mutex_destroy(&lock);
	printf("n = %d\n", n);
}