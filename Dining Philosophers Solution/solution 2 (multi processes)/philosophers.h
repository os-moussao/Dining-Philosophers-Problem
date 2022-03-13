/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 18:48:23 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/13 18:35:56 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>
# include <unistd.h>
# include <signal.h>
# include <semaphore.h>
# include <sys/time.h>

typedef struct timeval	t_timeval;

typedef struct s_data
{
	int				nphilo;
	unsigned long	pstart;
	int				max_meals;
	useconds_t		time_to_eat;
	useconds_t		time_to_sleep;
	useconds_t		time_to_die;
	pid_t			*pids;
	sem_t			*out;
	sem_t			*sem;
}				t_data;

typedef struct s_philo
{
	int				id;
	unsigned long	last_meal;
	int				meals_eaten;
	t_data			*pdata;
}				t_philo;

unsigned long	get_ms(void);
unsigned long	time_ms(unsigned long start);
int				ft_atoi(const char *str);
void			out(char *action, t_philo *philo);
void			my_usleep(unsigned long time);
void			killall(int *pids, int n);
int				my_wait(t_data pdata, int childs);

#endif