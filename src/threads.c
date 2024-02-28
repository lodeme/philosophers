/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis.demetz <louis.demetz@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 23:09:15 by louis.demet       #+#    #+#             */
/*   Updated: 2024/02/28 23:09:35 by louis.demet      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int create_threads(t_data *data)
{
	int i;

	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_create(&data->thread[i], NULL, philosopher_cycle, (void *)data->philo[i]))
			return (ft_error(data, 3));
		i++;
	}
	if (pthread_create(&data->thread[i], NULL, monitor_cycle, (void *)data))
		return (ft_error(data, 3));
	return (SUCCESS);
}

void join_threads(t_data *data)
{
	int i;

	i = 0;
	while (i < data->philo_count + 1)
		pthread_join(data->thread[i++], NULL);
}

void destroy_mutexes(t_data *data)
{
	int i;

	i = 0;
	while (i < data->philo_count)
		pthread_mutex_destroy(&data->mutex[i++]);
}

int philosophers(t_data *data)
{
	if (!create_threads(data))
		return (FAILURE);
	join_threads(data);
	destroy_mutexes(data);
	return (SUCCESS);
}
