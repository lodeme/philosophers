/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycles.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piuser <piuser@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 13:00:53 by lodemetz          #+#    #+#             */
/*   Updated: 2024/03/04 00:58:13 by piuser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	log_state(t_philo *philo, int step)
{
	int	id;

	id = philo->id;
	pthread_mutex_lock(philo->data->continue_mutex);
	if (!philo->data->continue_sim)
		;
	else if (step == 1)
		printf("%lli\t%d is thinking.\n", ms_elapsed(philo->data), id);
	else if (step == 2)
		printf("%lli\t%d has taken a fork.\n", ms_elapsed(philo->data), id);
	else if (step == 3)
		printf("%lli\t%d is eating.\n", ms_elapsed(philo->data), id);
	else if (step == 4)
		printf("%lli\t%d is sleeping.\n", ms_elapsed(philo->data), id);
	pthread_mutex_unlock(philo->data->continue_mutex);
}

void	lock_mutex(t_philo *philo)
{
	if (philo->id == 0)
	{
		log_state(philo, 1);
		pthread_mutex_lock(&philo->data->mutex[philo->id]);
		log_state(philo, 2);
		pthread_mutex_lock(&philo->data->mutex[(philo->id + 1) \
			% philo->data->philo_count]);
	}
	else
	{
		log_state(philo, 1);
		pthread_mutex_lock(&philo->data->mutex[(philo->id + 1) \
			% philo->data->philo_count]);
		log_state(philo, 2);
		pthread_mutex_lock(&philo->data->mutex[philo->id]);
	}
	log_state(philo, 2);
	log_state(philo, 3);
}

int	access_mutex(int *var, pthread_mutex_t *mutex)
{
	int	value;

	pthread_mutex_lock(mutex);
	value =	*var;
	pthread_mutex_unlock(mutex);
	return (value);
}

void	*philosopher_cycle(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 1)
		usleep(philo->data->ms_to_eat * 1000);
	while (access_mutex(&philo->data->continue_sim, philo->data->continue_mutex))
	{
		lock_mutex(philo);
		if (access_mutex(&philo->data->continue_sim, philo->data->continue_mutex))
		{
			usleep(philo->data->ms_to_eat * 1000);
			pthread_mutex_lock(philo->data->meal_count_mutex);
			philo->meal_count++;
			philo->last_meal_ts = ts();
			pthread_mutex_unlock(philo->data->meal_count_mutex);
		}
		pthread_mutex_unlock(&philo->data->mutex[philo->id]);
		pthread_mutex_unlock(&philo->data->mutex[(philo->id + 1) % \
			philo->data->philo_count]);
		log_state(philo, 4);
		if (access_mutex(&philo->data->continue_sim, philo->data->continue_mutex))
			usleep(philo->data->ms_to_sleep * 1000);
	}
	pthread_exit(NULL);
}

void	check_philosopher_state(t_data *data, long long current_ts)
{
	int	i;
	int	satiated;

	i = 0;
	satiated = 0;
	pthread_mutex_lock(data->continue_mutex);
	pthread_mutex_lock(data->meal_count_mutex);
	while (i < data->philo_count && data->continue_sim)
	{
		if ((current_ts - data->philo[i]->last_meal_ts) > (data->ms_to_starve))
		{
			data->continue_sim = 0;
			printf("%lli\t%i died\n", ms_elapsed(data), i);
		}
		if (data->philo[i]->meal_count >= data->times_eating)
			satiated++;
		i++;
	}
	pthread_mutex_unlock(data->meal_count_mutex);
	if (data->times_eating && satiated >= data->philo_count)
	{
		data->continue_sim = 0;
		printf("%lli\tAll philosophers have had %i meals\n", \
			ms_elapsed(data), data->times_eating);
	}
	pthread_mutex_unlock(data->continue_mutex);
}

void	*monitor_cycle(void *arg)
{
	t_data		*data;
	long long	current_ts;

	data = (t_data *)arg;
	while (access_mutex(&data->continue_sim, data->continue_mutex))
	{
		current_ts = ts();
		check_philosopher_state(data, current_ts);
		usleep(50);
	}
	pthread_exit(NULL);
}
