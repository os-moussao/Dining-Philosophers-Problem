/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 18:45:17 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/13 18:34:58 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosophers.h"

void	*routine(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	while (1)
	{
		sem_wait(philo->pdata->sem);
		out("has taken a fork", philo);
		sem_wait(philo->pdata->sem);
		out("has taken a fork", philo);
		out("is eating", philo);
		my_usleep(philo->pdata->time_to_eat);
		philo->last_meal = get_ms();
		(philo->meals_eaten)++;
		sem_post(philo->pdata->sem);
		sem_post(philo->pdata->sem);
		if (philo->pdata->max_meals != -1 && \
		philo->meals_eaten >= philo->pdata->max_meals)
			return (ptr);
		out("is sleeping", philo);
		my_usleep(philo->pdata->time_to_sleep);
		out("is thinking", philo);
	}
	return (ptr);
}

t_data	parse(char **av)
{
	t_data	pdata;

	pdata.nphilo = ft_atoi(av[0]);
	pdata.time_to_die = (useconds_t)ft_atoi(av[1]);
	pdata.time_to_eat = (useconds_t)ft_atoi(av[2]);
	pdata.time_to_sleep = (useconds_t)ft_atoi(av[3]);
	pdata.max_meals = ft_atoi(av[4]);
	pdata.pids = (pid_t *)malloc(pdata.nphilo * sizeof(pid_t));
	if (!pdata.pids)
		exit(1);
	sem_unlink("/sem_out");
	sem_unlink("/sem_forks");
	pdata.out = sem_open("/sem_out", O_CREAT, 0644, 1);
	pdata.sem = sem_open("/sem_forks", O_CREAT, 0644, pdata.nphilo);
	if (pdata.out == SEM_FAILED || pdata.sem == SEM_FAILED)
	{
		if (pdata.sem == SEM_FAILED)
			sem_close(pdata.out);
		write(2, "sem_open: sem failed\n", 21);
		free(pdata.pids);
		exit(2);
	}
	pdata.pstart = get_ms();
	return (pdata);
}

int	check(t_philo *philo)
{
	while (1)
	{
		if (philo->pdata->max_meals != -1 && \
		philo->meals_eaten >= philo->pdata->max_meals)
			return (EXIT_SUCCESS);
		if (get_ms() - philo->last_meal > \
		philo->pdata->time_to_die)
		{
			sem_wait(philo->pdata->out);
			printf("%ld %d has died\n", \
			time_ms(philo->pdata->pstart), philo->id);
			return (EXIT_FAILURE);
		}
		usleep(50);
	}
}

void	child(int id, t_data pdata)
{
	pthread_t	th;
	t_philo		philo;
	int			exit_status;

	philo.id = id;
	philo.last_meal = get_ms();
	philo.meals_eaten = 0;
	philo.pdata = &pdata;
	if (id & 1 && pdata.nphilo > 1)
		my_usleep(pdata.time_to_eat / 2);
	pthread_create(&th, NULL, routine, &philo);
	pthread_detach(th);
	exit_status = check(&philo);
	free(pdata.pids);
	exit(exit_status);
}

int	main(int ac, char **av)
{
	t_data	pdata;
	int		i;
	int		exit_status;

	if (ac < 5 || ac > 6)
		return (write(2, "Bad arguments\n", 14), 1);
	pdata = parse(av + 1);
	i = -1;
	while (++i < pdata.nphilo)
	{
		pdata.pids[i] = fork();
		if (pdata.pids[i] == 0)
			child(i + 1, pdata);
		else if (pdata.pids[i] == -1)
		{
			killall(pdata.pids, i);
			write(2, "fork: error\n", 12);
			exit(EXIT_FAILURE);
		}
	}
	exit_status = my_wait(pdata, pdata.nphilo);
	free(pdata.pids);
	sem_close(pdata.out);
	sem_close(pdata.sem);
	exit(exit_status);
}
