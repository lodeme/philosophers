/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis.demetz <louis.demetz@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:59:05 by louis.demet       #+#    #+#             */
/*   Updated: 2024/02/28 00:26:49 by louis.demet      ###   ########.fr       */
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
		while (data->philo[i])
			free(data->philo[i++]);
		free(data->philo);
	}
	if (data->thread)
		free(data->thread);
	if (data->mutex)
		free(data->mutex);
	free(data);
}

int	ft_error(t_data *data, int err)
{
	printf("Error:");
	if (err == 1)
		printf("Unable to allocate memory\n");
	if (err == 2)
		printf("Unable to initialize mutex\n");
	else
		printf("Unknown error\n");
	free_data(data);
	return (FAILURE);
}