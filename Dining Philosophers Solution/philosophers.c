#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

#define PHILOS			5
#define TIME_TO_EAT		200
#define TIME_TO_SLEEP	200
#define TIME_TO_DIE		400

typedef struct timeval t_timeval;

int	stop = 0;

typedef struct s_philo
{
	int				id;
	pthread_t		action;
	pthread_t		check;
	pthread_mutex_t	*forks;
	unsigned long	time_start;
	unsigned long	last_meal;
}				t_philo;


pthread_mutex_t	print_m;

/**
 * GET TIME IN MILISECONDS
 */

unsigned long	time_now(void)
{
	t_timeval	now;
	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}

unsigned long	get_ms(unsigned long start)
{
	return (time_now() - start);
}

/**
 * PRINT ACTION
 */
void	out(char *action, int i, unsigned long start)
{
	pthread_mutex_lock(&print_m);
	printf("Philosopher #%d %s at %lums\n", i, action, get_ms(start));
	pthread_mutex_unlock(&print_m);
}

/**
 * CHECK IF IS DYING
 */
void	*check_health(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	// while (philo->time_to_die);
	// stop = 1;
	// return ptr;
	while (1)
	{
		if (time_now() - philo->last_meal >= TIME_TO_DIE)
			break ;
	}
	out("has died", philo->id, philo->time_start);
	stop = 1;
	return (ptr);
}

/**
 * ACTION
 */
void	*eat(void *ptr)
{
	t_philo		*philo;
	t_timeval	sec;

	philo = (t_philo *)ptr;
	if (philo->id & 1) {
		sleep(TIME_TO_EAT);
	}

	pthread_create(&(philo->check), NULL, check_health, philo);
	pthread_detach(philo->check);

	while (1)
	{
		// hold ith and (i+1)th forks
		pthread_mutex_lock(&(philo->forks[philo->id]));
		out("has taken a fork", philo->id, philo->time_start);
		pthread_mutex_lock(&(philo->forks[(philo->id + 1) % PHILOS]));
		out("has taken a fork", philo->id, philo->time_start);

		// EAT
		out("is eating", philo->id, philo->time_start);
		usleep(TIME_TO_EAT * 1000);
		philo->last_meal = time_now();

		// put down ith and (i+1)th forks
		pthread_mutex_unlock(&(philo->forks[philo->id]));
		pthread_mutex_unlock(&(philo->forks[(philo->id + 1) % PHILOS]));

		// sleep
		out("is sleeping", philo->id, philo->time_start);
		usleep(TIME_TO_SLEEP * 1000);

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

	timestart = time_now();
	pthread_mutex_init(&print_m, NULL);
	forks = malloc(PHILOS * sizeof(pthread_mutex_t));
	for (int i = 0; i < PHILOS; i++) {
		pthread_mutex_init(&forks[i], NULL);
		philos[i].forks = forks;
		philos[i].id = i;
		philos[i].time_start = timestart;
		philos[i].last_meal = 0;
	}

	// open threads for philosophers
	for (int i = 0; i < PHILOS; i++) {
		pthread_create(&philos[i].action, NULL, eat, &philos[i]);
		pthread_detach(philos[i].action);
	}

	// wait until stop = 1
	while (!stop);

	for (int i = 0; i < PHILOS; i++)
		pthread_mutex_destroy(&forks[i]);
	pthread_mutex_destroy(&print_m);
	free(forks);
}