/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lodemetz <lodemetz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 23:09:15 by louis.demet       #+#    #+#             */
/*   Updated: 2024/03/05 17:03:31 by lodemetz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	create_processes(t_data *data)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i <= data->philo_count)
	{
		pid = fork();
		if (pid < 0)
			return (ft_error(data, 3));
		if (pid == 0 && i < data->philo_count)
			philosopher_cycle((void *)data->philo[i]);
		if (pid == 0 && i == data->philo_count)
			monitor_cycle((void *)data);
		i++;
	}
	return (SUCCESS);
}

void	wait_on_processes(t_data *data)
{
	int	i;

	i = 0;
	while (i <= data->philo_count)
	{
		wait(NULL);
		i++;
	}
}

int	philosophers(t_data *data)
{
	if (!create_processes(data))
		return (FAILURE);
	wait_on_processes(data);
	return (SUCCESS);
}
