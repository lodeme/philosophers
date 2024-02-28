/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis.demetz <louis.demetz@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 13:00:53 by lodemetz          #+#    #+#             */
/*   Updated: 2024/02/28 23:03:01 by louis.demet      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	log_state(t_philo *philo, int step)
{
	if (philo->data->continue_sim && step == 1)
		printf("%lli\t%d is thinking.\n", ms_elapsed(philo->data), philo->id);
	else if (philo->data->continue_sim && step == 2)
		printf("%lli\t%d has taken a fork.\n", ms_elapsed(philo->data), philo->id);
	else if (philo->data->continue_sim && step == 3)
		printf("%lli\t%d is eating.\n", ms_elapsed(philo->data), philo->id);
	else if (philo->data->continue_sim && step == 4)
		printf("%lli\t%d is sleeping.\n", ms_elapsed(philo->data), philo->id);
}

void	lock_mutex(t_philo *philo)
{
	if (philo->id == 0)
	{
		log_state(philo, 1);
		pthread_mutex_lock(&philo->data->mutex[philo->id]);
		log_state(philo, 2);
		pthread_mutex_lock(&philo->data->mutex[(philo->id + 1) % philo->data->philo_count]);
	}
	else
	{
		log_state(philo, 1);
		pthread_mutex_lock(&philo->data->mutex[(philo->id + 1) % philo->data->philo_count]);
		log_state(philo, 2);
		pthread_mutex_lock(&philo->data->mutex[philo->id]);
	}
	log_state(philo, 2);
	log_state(philo, 3);
}

void *philosopher_cycle(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	while (philo->data->continue_sim)
	{
		lock_mutex(philo);
		if (philo->data->continue_sim)
			usleep(philo->data->ms_to_eat * 1000);
		philo->meal_count++;
		philo->last_meal_ts = ts();
		pthread_mutex_unlock(&philo->data->mutex[philo->id]);
		pthread_mutex_unlock(&philo->data->mutex[(philo->id + 1) % \
			philo->data->philo_count]);
		log_state(philo, 4);
		if (philo->data->continue_sim)
			usleep(philo->data->ms_to_sleep * 1000);
	}
	pthread_exit(NULL);
}

void *monitor_cycle(void *arg)
{
	t_data *data;
	long long current_ts;
	int i;

	data = (t_data *)arg;
	while (data->continue_sim)
	{
		current_ts = ts();
		i = 0;
		while (i < data->philo_count)
		{
			if ((current_ts - data->philo[i]->last_meal_ts) > (data->ms_to_starve))
			{
				data->continue_sim = 0;
				printf("%lli\t%i died\n", ms_elapsed(data), i);
			}
			if (data->times_eating && data->philo[i]->meal_count >= data->times_eating)
			{
				data->continue_sim = 0;
				printf("%lli\tAll philosophers have eaten %i meals\n", ms_elapsed(data), data->times_eating);
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
