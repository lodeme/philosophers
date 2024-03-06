/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piuser <piuser@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:59:05 by louis.demet       #+#    #+#             */
/*   Updated: 2024/03/06 17:16:33 by piuser           ###   ########.fr       */
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
	if (data->forks_sem)
	{
		sem_close(data->forks_sem);
		sem_unlink("forks_sem");
	}
	if (data->continue_sem)
	{
		sem_close(data->continue_sem);
		sem_unlink("continue_sem");
	}
	if (data->message_sem)
	{
		sem_close(data->message_sem);
		sem_unlink("message_sem");
	}
	free(data);
}

int	ft_error(t_data *data, int err)
{
	printf("Error:");
	if (err == 1)
		printf("Unable to allocate memory\n");
	if (err == 2)
		printf("Unable to initialize semaphore\n");
	if (err == 3)
		printf("Unable to fork process\n");
	else
		printf("Unknown error\n");
	free_data(data);
	return (FAILURE);
}
