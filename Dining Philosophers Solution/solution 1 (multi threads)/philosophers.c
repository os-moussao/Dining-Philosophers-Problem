/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 18:45:17 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/13 15:26:40 by omoussao         ###   ########.fr       */
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
	t_philo	*philo;

	philo = (t_philo *)ptr;
	if (!(philo->id & 1) && philo->pdata->nphilo > 1)
		my_usleep(philo->pdata->time_to_eat / 2);
	while (1)
	{
		pthread_mutex_lock(philo->fork_right);
		out("has taken a fork", philo);
		pthread_mutex_lock(philo->fork_left);
		out("has taken a fork", philo);
		out("is eating", philo);
		my_usleep(philo->pdata->time_to_eat);
		philo->last_meal = time_ms(philo->pdata->pstart);
		(philo->meals_eaten)++;
		pthread_mutex_unlock(philo->fork_right);
		pthread_mutex_unlock(philo->fork_left);
		if (philo->pdata->max_meals != -1 && \
		philo->meals_eaten >= philo->pdata->max_meals)
			return ((philo->pdata->finished)++, ptr);
		out("is sleeping", philo);
		my_usleep(philo->pdata->time_to_sleep);
		out("is thinking", philo);
	}
	return (ptr);
}

t_data	parse(char **av, t_philo **philos, pthread_mutex_t **forks)
{
	t_data	pdata;
	int		i;

	memset(&pdata, 0, sizeof(pdata));
	pdata.nphilo = ft_atoi(av[0]);
	pdata.time_to_die = (useconds_t)ft_atoi(av[1]);
	pdata.time_to_eat = (useconds_t)ft_atoi(av[2]);
	pdata.time_to_sleep = (useconds_t)ft_atoi(av[3]);
	pdata.max_meals = ft_atoi(av[4]);
	*philos = (t_philo *)malloc(pdata.nphilo * sizeof(t_philo));
	*forks = (pthread_mutex_t *)malloc(pdata.nphilo * sizeof(pthread_mutex_t));
	if (!*philos || !*forks)
		return (pdata);
	pthread_mutex_init(&(pdata.print_m), NULL);
	i = -1;
	while (++i < pdata.nphilo)
	{
		pthread_mutex_init(*forks + i, NULL);
		(*philos)[i].meals_eaten = 0;
		(*philos)[i].fork_right = *forks + i;
		(*philos)[i].fork_left = *forks + (i + 1) % pdata.nphilo;
		(*philos)[i].id = i;
	}
	return (pdata);
}

void	check(t_data *pdata, t_philo *philos)
{
	int	i;

	while (1)
	{
		if (pdata->finished == pdata->nphilo)
			return ;
		i = -1;
		while (++i < pdata->nphilo)
		{
			if (pdata->max_meals != -1 && \
			philos[i].meals_eaten >= pdata->max_meals)
				continue ;
			if (time_ms(pdata->pstart) - philos[i].last_meal > \
			pdata->time_to_die)
			{
				pthread_mutex_lock(&(pdata->print_m));
				printf("%ld %d has died\n", time_ms(pdata->pstart), i + 1);
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
		return (write(2, "Bad arguments\n", 14), 1);
	pdata = parse(av + 1, &philos, &forks);
	if (!forks || !philos)
		return (free(forks), free(philos), 2);
	pdata.pstart = get_ms();
	i = -1;
	while (++i < pdata.nphilo)
	{
		philos[i].pdata = &pdata;
		philos[i].last_meal = 0;
		pthread_create(&(philos[i].th), NULL, routine, &philos[i]);
		pthread_detach(philos[i].th);
	}
	check(&pdata, philos);
	usleep(100);
	while (i--)
		pthread_mutex_destroy(&forks[i]);
	pthread_mutex_destroy(&(pdata.print_m));
	return (free(forks), free(philos), 0);
}
