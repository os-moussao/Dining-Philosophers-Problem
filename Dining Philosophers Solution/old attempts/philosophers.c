#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

#define PHILOS			4
#define TIME_TO_EAT		200
#define TIME_TO_SLEEP	200
#define TIME_TO_DIE		411
#define USEC(ms)		ms * 1000

typedef struct timeval t_timeval;

typedef struct s_philo
{
	int				id;
	pthread_t		action;
	pthread_t		check;
	pthread_mutex_t	*forks;
	unsigned long	time_start;
	unsigned long	last_meal[PHILOS];
}				t_philo;


pthread_mutex_t	print_m;

/**
 * GET TIME IN MILISECONDS
 */

unsigned long	get_ms(void)
{
	t_timeval	now;
	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}

unsigned long	time_ms(unsigned long start)
{
	return (get_ms() - start);
}

/**
 * PRINT ACTION
 */
void	out(char *action, int i, unsigned long start)
{
	pthread_mutex_lock(&print_m);
	printf("Philosopher #%d %s at %lums\n", i, action, time_ms(start));
	pthread_mutex_unlock(&print_m);
}

/**
 * ACTION
 */
void	*eat(void *ptr)
{
	t_philo		*philo;

	philo = (t_philo *)ptr;
	if (!(philo->id & 1)) {
		usleep(USEC(TIME_TO_EAT));
	}

	while (1)
	{
		//
		// hold ith and (i+1)th forks
		//
		pthread_mutex_lock(&(philo->forks[philo->id]));
		out("has taken a fork", philo->id, philo->time_start);
		pthread_mutex_lock(&(philo->forks[(philo->id + 1) % PHILOS]));
		out("has taken a fork", philo->id, philo->time_start);

		// EAT
		out("is eating", philo->id, philo->time_start);
		usleep(USEC(TIME_TO_EAT));
		philo->last_meal[philo->id] = time_ms(philo->time_start);

		//
		// put down ith and (i+1)th forks
		//
		pthread_mutex_unlock(&(philo->forks[philo->id]));
		pthread_mutex_unlock(&(philo->forks[(philo->id + 1) % PHILOS]));

		// sleep
		out("is sleeping", philo->id, philo->time_start);
		usleep(USEC(TIME_TO_SLEEP));

		// think
		out("is thinking", philo->id, philo->time_start);
	}

	return ptr;
}

/**
 * MAIN FUNCTION
 */
int main()
{
	t_philo			philos[PHILOS];
	pthread_mutex_t	*forks;
	unsigned long	timestart;

	timestart = get_ms();
	pthread_mutex_init(&print_m, NULL);
	forks = malloc(PHILOS * sizeof(pthread_mutex_t));
	for (int i = 0; i < PHILOS; i++) {
		pthread_mutex_init(&forks[i], NULL);
		philos[i].forks = forks;
		philos[i].id = i;
		philos[i].time_start = timestart;
		philos[i].last_meal[i] = 0;
	}

	// open threads for philosophers
	for (int i = 0; i < PHILOS; i++) {
		pthread_create(&philos[i].action, NULL, eat, &philos[i]);
		pthread_detach(philos[i].action);
	}

	/**
	 * CHECK IF SOME PHILOSOPHER IS DYING
	*/
	int stop = 0;
	while (!stop) {
		for (int i = 0; i < PHILOS; i++) {
			if (time_ms(timestart) > TIME_TO_DIE + philos[i].last_meal[i]) {
				out("has died", i, timestart);
				pthread_mutex_lock(&print_m);
				printf("Difference of time %lu\n", time_ms(timestart) - philos[i].last_meal[i]);
				stop = 1;
				break ;
			}
		}
	}

	for (int i = 0; i < PHILOS; i++)
		pthread_mutex_destroy(&forks[i]);
	pthread_mutex_destroy(&print_m);
	free(forks);
}