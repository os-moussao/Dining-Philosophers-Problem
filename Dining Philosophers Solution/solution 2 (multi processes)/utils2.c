/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omoussao <omoussao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 18:35:09 by omoussao          #+#    #+#             */
/*   Updated: 2022/03/13 18:36:45 by omoussao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	my_wait(t_data pdata, int childs)
{
	int	status;

	if (!childs)
		return (EXIT_SUCCESS);
	wait(&status);
	if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
		return (my_wait(pdata, childs - 1));
	return (killall(pdata.pids, pdata.nphilo), EXIT_FAILURE);
}
