/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 18:45:17 by omoussao          #+#    #+#             */
/*   Updated: 2022/01/19 20:11:46 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

// #define PHILOS			4
// #define TIME_TO_EAT		200
// #define TIME_TO_SLEEP	200
// #define TIME_TO_DIE		411

#define USEC(ms)	ms * 1000
#define LEFT(i, N)	(i + 1) % N

typedef struct timeval t_timeval;

typedef struct s_philo
{
	int				id;
	int				N;
	pthread_t		th;
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;
	pthread_mutex_t	*print_m;
	unsigned long	last_meal;
	unsigned long	pstart;
	useconds_t		time_to_eat;
	useconds_t		time_to_sleep;
	useconds_t		time_to_die;
	int				max_meals;
	
}				t_philo;

static int	is_w_space(char c)
{
	return (c == 32 || (c >= 9 && c <= 13));
}

int	ft_atoi(const char *str)
{
	int	res;
	int	sign;
	int	i;

	res = 0;
	i = 0;
	while (is_w_space(str[i]))
		i++;
	sign = 1 - (str[i] == '-') * 2;
	i += (str[i] == '+' || str[i] == '-');
	while (str[i] && str[i] >= '0' && str[i] <= '9')
		res = res * 10 + str[i++] - '0';
	return (sign * res);
}

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
	pthread_mutex_lock(philo->print_m);
	printf("%lu %d %s\n", time_ms(philo->pstart), philo->id + 1, action);
	pthread_mutex_unlock(philo->print_m);
}

/**
 * THIS IS THE MAIN ROUTINE OF EVERY PHILOSOPHER
 * 
 * ONLY AT THE BEGINING IF THE ID IS EVEN IT WAITS (AVOIDING DEADLOCK)
 * 
 * TAKE THE FORKS ON THE LEFT AND ON THE RIGHT
 * EAT time_to_eat milliseconds
 * PUT DOWN THE FORKS
 * SLEEP time_to_sleep milliseconds
 * THINK, REPEAT
 */
void	*routine(void *ptr)
{
	t_philo		*philo;

	philo = (t_philo *)ptr;
	if (!(philo->id & 1))
		usleep(USEC(philo->time_to_eat));
	while (1)
	{
		pthread_mutex_lock(philo->fork_right);
		out("has taken a fork", philo);
		pthread_mutex_lock(philo->fork_left);
		out("has taken a fork", philo);
		out("is eating", philo);
		usleep(USEC(philo->time_to_eat));
		philo->last_meal = time_ms(philo->pstart);
		pthread_mutex_unlock(philo->fork_right);
		pthread_mutex_unlock(philo->fork_left);
		out("is sleeping", philo);
		usleep(USEC(philo->time_to_sleep));
		out("is thinking", philo);
	}
	return ptr;
}

void	parse(int ac, char **av, t_philo **philos, pthread_mutex_t **forks)
{
	int			N;
	int			max_meals;
	useconds_t	time_to_eat;
	useconds_t	time_to_sleep;
	useconds_t	time_to_die;

	N = ft_atoi(av[0]);
	time_to_die = (useconds_t)ft_atoi(av[1]);
	time_to_eat = (useconds_t)ft_atoi(av[2]);
	time_to_sleep = (useconds_t)ft_atoi(av[3]);
	max_meals = -1;
	if (ac == 5)
		max_meals = ft_atoi(av[4]);
	*philos = (t_philo *)malloc(N * sizeof(t_philo));
	*forks = (pthread_mutex_t *)malloc(N * sizeof(pthread_mutex_t));
	
}

int	main(int ac, char **av)
{
	t_philo			*philos;
	pthread_mutex_t	*forks;
	unsigned long	timestart;

	if (ac < 5 || ac > 6)
	{
		write(2, "Bad arguments\n", 14);
		return (1);
	}
	parse(ac - 1, av + 1, &philos, &forks);
	N = ft_atoi(av[1]);
	philos = (t_philo *)malloc(N * sizeof(t_philo));
	forks = (pthread_mutex_t *)malloc(N * sizeof(pthread_mutex_t));

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