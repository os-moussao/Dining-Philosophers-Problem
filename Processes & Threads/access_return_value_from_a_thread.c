#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void	*get_random_v(void *ptr) {
	static int val;
	
	val = (rand() % 42) + 1;
	printf("val = %d\n", val);
	return &val;
}

int main(int ac, char **av)
{
	srand(time(NULL));
	pthread_t th;
	int *res;

	pthread_create(&th, NULL, get_random_v, NULL);
	pthread_join(th, (void **) &res);
	printf("res = %d\n", *res);
}