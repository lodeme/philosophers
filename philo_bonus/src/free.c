/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lodemetz <lodemetz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:59:05 by louis.demet       #+#    #+#             */
/*   Updated: 2024/03/08 19:32:11 by lodemetz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	close_semaphores(t_data *data)
{
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
	if (data->meals_sem)
	{
		sem_close(data->meals_sem);
		sem_unlink("meals_sem");
	}
}

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
	close_semaphores(data);
	free(data);
}

int	ft_error(t_data *data, int err)
{
	printf("Error: ");
	if (err == 1)
		printf("Unable to allocate memory\n");
	else if (err == 2)
		printf("Unable to initialize semaphore\n");
	else if (err == 3)
		printf("Unable to fork process\n");
	else if (err == 4)
		printf("Wrong parameter values\n");
	else if (err == 5)
		printf("Unable to create thread\n");
	else
		printf("Unknown error\n");
	free_data(data);
	return (FAILURE);
}
