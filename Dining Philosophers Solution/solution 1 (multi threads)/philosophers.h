/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 18:48:23 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/12 20:50:50 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>
# include <semaphore.h>
# include <sys/time.h>

typedef struct timeval	t_timeval;

typedef struct s_data
{
	int				nphilo;
	int				finished;
	unsigned long	pstart;
	int				max_meals;
	useconds_t		time_to_eat;
	useconds_t		time_to_sleep;
	useconds_t		time_to_die;
	pthread_mutex_t	print_m;
}				t_data;

typedef struct s_philo
{
	int				id;
	pthread_t		th;
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;
	unsigned long	last_meal;
	int				meals_eaten;
	t_data			*pdata;
}				t_philo;

unsigned long	get_ms(void);
unsigned long	time_ms(unsigned long start);
int				ft_atoi(const char *str);
void			out(char *action, t_philo *philo);
void			my_usleep(unsigned long time);

#endif