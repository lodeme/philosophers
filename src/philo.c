/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lodemetz <lodemetz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 13:00:53 by lodemetz          #+#    #+#             */
/*   Updated: 2024/02/28 13:05:39 by lodemetz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philosopher_lifecycle(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		printf("%lli %d is thinking.\n", ms_elapsed(philo->data), philo->id);
		pthread_mutex_lock(&philo->data->mutex[philo->id]);
		printf("%lli %d has taken a fork.\n", ms_elapsed(philo->data), philo->id);
		pthread_mutex_lock(&philo->data->mutex[(philo->id + 1) % philo->data->philo_count]);
		printf("%lli %d has taken a fork.\n", ms_elapsed(philo->data), philo->id);
		printf("%lli %d is eating.\n", ms_elapsed(philo->data), philo->id);
		usleep(philo->data->ms_to_eat * 1000);
		pthread_mutex_unlock(&philo->data->mutex[philo->id]);
		pthread_mutex_unlock(&philo->data->mutex[(philo->id + 1) % philo->data->philo_count]);
		printf("%lli %d is sleeping.\n", ms_elapsed(philo->data), philo->id);
		usleep(philo->data->ms_to_sleep * 1000);
	}
	pthread_exit(NULL);
}

int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_create(&data->thread[i], NULL, philosopher_lifecycle, (void *)data->philo[i]))
			return (ft_error(data, 3));
		i++;
	}
	return (SUCCESS);
}

void	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
		pthread_join(data->thread[i++], NULL);
}

void	destroy_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
		pthread_mutex_destroy(&data->mutex[i++]);
}

int	philosophers(t_data *data)
{
	if (!create_threads(data))
		return (FAILURE);
	join_threads(data);
	destroy_mutexes(data);
	return (SUCCESS);
}
