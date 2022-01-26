#include "./philosophers.h"

/**
 * GET TIME IN MILLISECONDS
 */
unsigned long	get_ms(void)
{
	t_timeval	now;
	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}

/**
 * GET TIME IN MILLISECONDS
 * RELATIVE TO THE START OF THE PROGRAM
 */
unsigned long	time_ms(unsigned long start)
{
	return (get_ms() - start);
}

/**
 * PRINT WHEN print_m IS UNLOCKED
 */
void	out(char *action, t_philo *philo)
{
	pthread_mutex_lock(&(philo->pdata->print_m));
	printf("%lu %d %s\n", time_ms(philo->pdata->pstart), philo->id + 1, action);
	pthread_mutex_unlock(&(philo->pdata->print_m));
}