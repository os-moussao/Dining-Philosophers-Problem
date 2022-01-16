#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int bin[10] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512};

typedef struct s_var {
	int *start;
	int n;
	int sum;
} t_var;

void	*routine(void *ptr)
{
	t_var *var = (t_var *)ptr;
	var->sum = 0;
	for (int i = 0; i < var->n; i++) {
		var->sum += *(var->start + i);
	}
	return ptr;
}

int main(int ac, char **av)
{
	pthread_t t1, t2;
	t_var *v1 = malloc(sizeof(t_var));
	t_var *v2 = malloc(sizeof(t_var));
	v1->start = bin; v1->n = 5;
	v2->start = bin + 5; v2->n = 5;
	pthread_create(&t1, NULL, routine, (void *)v1);
	pthread_create(&t2, NULL, routine, (void *)v2);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	printf("sum = %d\n", v1->sum + v2->sum);
	free(v1);
	free(v2);
	printf("the right answer is 1023\n");
}