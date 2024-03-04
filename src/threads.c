/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lodemetz <lodemetz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 23:09:15 by louis.demet       #+#    #+#             */
/*   Updated: 2024/03/04 16:59:53 by lodemetz         ###   ########.fr       */
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
		if (pid < 0 && i < data->philo_count)
			
		if (pthread_create(&data->thread[i], NULL, philosopher_cycle, (void *)data->philo[i]))
			return (ft_error(data, 3));
		i++;
	}
	if (pthread_create(&data->thread[i], NULL, monitor_cycle, (void *)data))
		return (ft_error(data, 3));
	return (SUCCESS);
}

void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count + 1)
		pthread_join(data->thread[i++], NULL);
}


int	philosophers(t_data *data)
{
	if (!create_threads(data))
		return (FAILURE);
	join_threads(data);
	return (SUCCESS);
}
