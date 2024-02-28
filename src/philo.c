/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis.demetz <louis.demetz@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 13:00:53 by lodemetz          #+#    #+#             */
/*   Updated: 2024/02/28 21:14:13 by louis.demet      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philosopher_cycle(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (philo->data->sim_ok)
	{
		printf("%lli %d is thinking.\n", ms_elapsed(philo->data), philo->id);
		pthread_mutex_lock(&philo->data->mutex[philo->id]);
		printf("%lli %d has taken a fork.\n", ms_elapsed(philo->data), philo->id);
		pthread_mutex_lock(&philo->data->mutex[(philo->id + 1) % philo->data->philo_count]);
		printf("%lli %d has taken a fork.\n", ms_elapsed(philo->data), philo->id);
		printf("%lli %d is eating.\n", ms_elapsed(philo->data), philo->id);
		usleep(philo->data->ms_to_eat * 1000);
		philo->meal_count++;
		philo->last_meal_ts = ts();
		pthread_mutex_unlock(&philo->data->mutex[philo->id]);
		pthread_mutex_unlock(&philo->data->mutex[(philo->id + 1) % philo->data->philo_count]);
		printf("%lli %d is sleeping.\n", ms_elapsed(philo->data), philo->id);
		usleep(philo->data->ms_to_sleep * 1000);
	}
	pthread_exit(NULL);
}

void	*monitor_cycle(void *arg)
{
	t_data		*data;
	long long	current_ts;
	int			i;

	data = (t_data *)arg;
	while (data->sim_ok)
	{
		current_ts = ts();
		i = 0;
		while (i < data->philo_count)
		{
			if ((current_ts - data->philo[i]->last_meal_ts) > (data->ms_to_starve))
			{
				data->sim_ok = 0;
				printf("%lli %i has starved\n", ms_elapsed(data), i);
			}
			i++;
		}
		usleep(1000);
	}
	pthread_exit(NULL);
}

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
