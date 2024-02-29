/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycles.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis.demetz <louis.demetz@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 13:00:53 by lodemetz          #+#    #+#             */
/*   Updated: 2024/02/29 09:08:37 by louis.demet      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	log_state(t_philo *philo, int step)
{
	int	id;

	id = philo->id;
	if (philo->data->continue_sim && step == 1)
		printf("%lli\t%d is thinking.\n", ms_elapsed(philo->data), id);
	else if (philo->data->continue_sim && step == 2)
		printf("%lli\t%d has taken a fork.\n", ms_elapsed(philo->data), id);
	else if (philo->data->continue_sim && step == 3)
		printf("%lli\t%d is eating.\n", ms_elapsed(philo->data), id);
	else if (philo->data->continue_sim && step == 4)
		printf("%lli\t%d is sleeping.\n", ms_elapsed(philo->data), id);
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

void *philosopher_cycle(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 1)
		usleep(50);
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

void	check_philosopher_state(t_data *data, long long current_ts)
{
	int	i;
	int	satiated;

	i = 0;
	satiated = 0;
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
	if (data->times_eating && satiated >= data->times_eating)
	{
		data->continue_sim = 0;
		printf("%lli\tAll philosophers have eaten %i meals\n", \
			 ms_elapsed(data), data->times_eating);
	}
}

void *monitor_cycle(void *arg)
{
	t_data *data;
	long long current_ts;

	data = (t_data *)arg;
	while (data->continue_sim)
	{
		current_ts = ts();
		check_philosopher_state(data, current_ts);
		usleep(1000);
	}
	pthread_exit(NULL);
}
