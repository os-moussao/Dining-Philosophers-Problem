/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 19:55:39 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/13 18:33:26 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	sem_wait(philo->pdata->out);
	printf("%lu %d %s\n", time_ms(philo->pdata->pstart), philo->id, action);
	sem_post(philo->pdata->out);
}

/**
 * MY VERSION OF USLEEP FUNCTION
 */
void	my_usleep(unsigned long time)
{
	unsigned long	curr;

	curr = get_ms();
	while (get_ms() - curr < time)
		usleep(50);
}

/**
 * KILL LIST OF PIDS
 */
void	killall(int *pids, int n)
{
	int	i;

	i = -1;
	while (++i < n)
		kill(pids[i], SIGKILL);
}
