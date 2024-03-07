/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piuser <piuser@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:59:05 by louis.demet       #+#    #+#             */
/*   Updated: 2024/03/04 01:04:52 by piuser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_data(t_data *data)
{
	int	i;

	i = 0;
	if (!data)
		return ;
	if (data->philo)
	{
		while (i < data->philo_count)
			free(data->philo[i++]);
		free(data->philo);
	}
	if (data->thread)
		free(data->thread);
	if (data->mutex)
		free(data->mutex);
	if (data->continue_mutex)
		free(data->continue_mutex);
	if (data->meal_count_mutex)
		free(data->meal_count_mutex);
	free(data);
}

int	ft_error(t_data *data, int err)
{
	printf("Error:");
	if (err == 1)
		printf("Unable to allocate memory\n");
	if (err == 2)
		printf("Unable to initialize mutex\n");
	if (err == 3)
		printf("Unable to create thread\n");
	else
		printf("Unknown error\n");
	free_data(data);
	return (FAILURE);
}
