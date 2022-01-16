#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int bin[10] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512};

void	*routine(void *ptr)
{
	printf("%d  ", bin[*(int *)ptr]);
	return ptr;
}

int main(int ac, char **av)
{
	pthread_t th[10];
	int *arr[10];
	
	for (int i=0; i < 10; i++) {
		arr[i] = malloc(sizeof(int));
		*(arr[i]) = i;
		if (pthread_create(&th[i], NULL, routine, arr[i]) != 0) {
			printf("Error\n");
			return 1;
		}
	}
	
	for (int i=0; i < 10; i++) {
		free(arr[i]);
		if (pthread_join(th[i], NULL) != 0) {
			printf("Error\n");
			return 2;
		}
	}
}