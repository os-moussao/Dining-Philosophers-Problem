/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: os-moussao <os-moussao@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 18:45:17 by omoussao          #+#    #+#             */
/*   Updated: 2022/01/26 14:51:44 by os-moussao       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosophers.h"

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
	philo->last_meal = get_ms();
	if (philo->id & 1)
		usleep(USEC(philo->pdata->time_to_eat));
	while (1)
	{
		pthread_mutex_lock(philo->fork_right);
		out("has taken a fork", philo);
		pthread_mutex_lock(philo->fork_left);
		out("has taken a fork", philo);
		out("is eating", philo);
		usleep(USEC(philo->pdata->time_to_eat));
		philo->last_meal = get_ms();
		(philo->meals_eaten)++;
		pthread_mutex_unlock(philo->fork_right);
		pthread_mutex_unlock(philo->fork_left);
		if (philo->pdata->max_meals != -1 && philo->meals_eaten >= philo->pdata->max_meals)
		{
			(philo->pdata->finished)++;
			return ptr;
		}
		out("is sleeping", philo);
		usleep(USEC(philo->pdata->time_to_sleep));
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
	pdata.finished = 0;
	pdata.max_meals = -1;
	if (ac == 5)
		pdata.max_meals = ft_atoi(av[4]);
	*philos = (t_philo *)malloc(pdata.N * sizeof(t_philo));
	*forks = (pthread_mutex_t *)malloc(pdata.N * sizeof(pthread_mutex_t));
	pthread_mutex_init(&(pdata.print_m), NULL);
	i = -1;
	while (++i < pdata.N)
	{
		pthread_mutex_init(*forks + i, NULL);
		(*philos)[i].meals_eaten = 0;
		(*philos)[i].fork_right = *forks + i;
		(*philos)[i].fork_left = *forks + (i + 1) % pdata.N;
		(*philos)[i].id = i;
	}
	return (pdata);
}

void	check(t_data *pdata, t_philo *philos)
{
	int	i;

	usleep(USEC(pdata->time_to_die / 2));
	while (1)
	{
		if (pdata->finished == pdata->N)
			return ;
		i = -1;
		while (++i < pdata->N)
		{
			if (pdata->max_meals != -1
				&& philos[i].meals_eaten >= pdata->max_meals)
				continue ;
			if (get_ms() > pdata->time_to_die + philos[i].last_meal)
			{
				out("has died", &philos[i]);
				exit(1);
				pthread_mutex_lock(&(pdata->print_m));
				return ;
			}
		}
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
		philos[i].pdata = &pdata;
		pthread_create(&(philos[i].th), NULL, routine, &philos[i]);
		pthread_detach(philos[i].th);
	}
	check(&pdata, philos);
	i = -1;
	while (++i < pdata.N)
		pthread_mutex_destroy(&forks[i]);
	pthread_mutex_destroy(&(pdata.print_m));
	free(forks);
	free(philos);
}