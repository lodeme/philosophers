/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis.demetz <louis.demetz@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:34:42 by louis.demet       #+#    #+#             */
/*   Updated: 2024/02/28 00:30:04 by louis.demet      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void *philosopher_lifecycle(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	while (1)
	{
		printf("%lli %d is thinking.\n", ms_elapsed(philo->data), philo->id);
		pthread_mutex_lock(&philo->data->mutex[philo->id]);
		printf("%lli %d has taken a fork.\n", ms_elapsed(philo->data), philo->id);
		pthread_mutex_lock(&philo->data->mutex[(philo->id + 1) % philo->data->philo_count]);
		printf("%lli %d has taken a fork.\n", ms_elapsed(philo->data), philo->id);
		printf("%lli %d is eating.\n", ms_elapsed(philo->data), philo->id);
		sleep(1);
		pthread_mutex_unlock(&philo->data->mutex[philo->id]);
		pthread_mutex_unlock(&philo->data->mutex[(philo->id + 1) % philo->data->philo_count]);
		printf("%lli %d is sleeping.\n", ms_elapsed(philo->data), philo->id);
		sleep(2);
	}
	pthread_exit(NULL);
}

int philosophers(t_data *data)
{
	// Create philosopher threads
	for (int i = 0; i < data->philo_count; i++)
	{
		if (pthread_create(&data->thread[i], NULL, philosopher_lifecycle, (void *)&data->philo[i]))
		{
			fprintf(stderr, "Error creating thread\n");
			return (FAILURE);
		}
	}

	// Join threads
	for (int i = 0; i < data->philo_count; i++)
		pthread_join(data->thread[i], NULL);

	// Destroy mutexes
	for (int i = 0; i < data->philo_count; i++)
		pthread_mutex_destroy(&data->mutex[i]);
	return (SUCCESS);
}

int main(int argc, char **argv)
{
	t_data	*data;

	if (argc < 5 || argc > 6)
	{
		printf("args error: expected:\n- nb_philosophers\n- ms_to_starve\n- ms_to_eat\n- ms_to_sleep\n- times_eating (optional)\n");
		return (EXIT_FAILURE);
	}
	init_data(&data, argv);
	if (!data)
		return (EXIT_FAILURE);
	if (!philosophers(data))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
