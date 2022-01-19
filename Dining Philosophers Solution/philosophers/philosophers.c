/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 18:45:17 by omoussao          #+#    #+#             */
/*   Updated: 2022/01/19 21:37:03 by omoussao         ###   ########.fr       */
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

typedef struct timeval t_timeval;

typedef struct s_data
{
	int				N;
	unsigned long	pstart;
	int				max_meals;
	useconds_t		time_to_eat;
	useconds_t		time_to_sleep;
	useconds_t		time_to_die;
	pthread_mutex_t	*print_m;
}				t_data;

typedef struct s_philo
{
	int				id;
	pthread_t		th;
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;
	unsigned long	last_meal;
	int				meals_eaten;
	t_data			pdata;
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
	pthread_mutex_lock(philo->pdata.print_m);
	printf("%lu %d %s\n", time_ms(philo->pdata.pstart), philo->id + 1, action);
	pthread_mutex_unlock(philo->pdata.print_m);
}

/**
 * THIS IS THE MAIN ROUTINE OF EVERY PHILOSOPHER
 * 
 * ONLY AT THE BEGINING IF THE ID IS ODD IT WAITS (AVOIDING DEADLOCK)
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
	if (philo->id & 1)
		usleep(USEC(philo->pdata.time_to_eat));
	while (1)
	{
		pthread_mutex_lock(philo->fork_right);
		out("has taken a fork", philo);
		pthread_mutex_lock(philo->fork_left);
		out("has taken a fork", philo);
		out("is eating", philo);
		usleep(USEC(philo->pdata.time_to_eat));
		philo->last_meal = get_ms();
		(philo->meals_eaten)++;
		pthread_mutex_unlock(philo->fork_right);
		pthread_mutex_unlock(philo->fork_left);
		out("is sleeping", philo);
		usleep(USEC(philo->pdata.time_to_sleep));
		out("is thinking", philo);
	}
	return ptr;
}

t_data	parse(int ac, char **av, t_philo **philos, pthread_mutex_t **forks)
{
	t_data	pdata;
	int		i;
	
	pdata.N = ft_atoi(av[0]);
	pdata.time_to_die = (useconds_t)ft_atoi(av[1]);
	pdata.time_to_eat = (useconds_t)ft_atoi(av[2]);
	pdata.time_to_sleep = (useconds_t)ft_atoi(av[3]);
	pdata.max_meals = -1;
	if (ac == 5)
		pdata.max_meals = ft_atoi(av[4]);
	*philos = (t_philo *)malloc(pdata.N * sizeof(t_philo));
	*forks = (pthread_mutex_t *)malloc(pdata.N * sizeof(pthread_mutex_t));
	pthread_mutex_init(pdata.print_m, NULL);
	i = -1;
	while (++i)
	{
		pthread_mutex_init(*forks + i, NULL);
		(*philos)[i].fork_right = &(*forks)[i];
		(*philos)[i].fork_left = &(*forks)[(i + 1) % pdata.N];
		(*philos)[i].id = i;
		(*philos)[i].meals_eaten = 0;
	}
	return (pdata);
}

void	check(t_data pdata, t_philo *philos)
{
	int	i;
	int	meals;

	while (1)
	{
		meals = 0;
		i = -1;
		while (++i < pdata.N)
		{
			if (get_ms() > pdata.time_to_die + get_ms())
			{
				out("has died", &philos[i]);
				pthread_mutex_lock(pdata.print_m);
				return ;
			}
			meals += philos[i].meals_eaten;
		}
		if (pdata.max_meals != -1 && meals >= pdata.max_meals * pdata.N)
			return ;
	}
}

int	main(int ac, char **av)
{
	t_data			pdata;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	int				i;

	if (ac < 5 || ac > 6)
	{
		write(2, "Bad arguments\n", 14);
		return (1);
	}
	pdata = parse(ac - 1, av + 1, &philos, &forks);
	pdata.pstart = get_ms();
	i = -1;
	while (++i < pdata.N)
	{
		philos[i].pdata = pdata;
		philos[i].last_meal = get_ms();
		pthread_create(&(philos[i].th), NULL, routine, &philos[i]);
		pthread_detach(philos[i].th);
	}
	check(pdata, philos);
	i = -1;
	while (++i < pdata.N)
		pthread_mutex_destroy(&forks[i]);
	pthread_mutex_destroy(pdata.print_m);
	free(forks);
	free(philos);
}