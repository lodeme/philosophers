/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycles.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lodemetz <lodemetz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 13:00:53 by lodemetz          #+#    #+#             */
/*   Updated: 2024/03/05 17:20:50 by lodemetz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	log_state(t_philo *philo, int step)
{
	int	id;

	id = philo->id;
	sem_wait(philo->data->forks_sem);
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
	sem_post(philo->data->continue_sem);
}

void	lock_mutex(t_philo *philo)
{
	log_state(philo, 1);
	sem_wait(philo->data->forks_sem);
	log_state(philo, 2);
	sem_wait(philo->data->forks_sem);
	log_state(philo, 2);
	log_state(philo, 3);
}

void	philosopher_cycle(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 1)
		usleep(philo->data->ms_to_eat * 1000);
	while (access_sem(&philo->data->continue_sim, philo->data->continue_sem))
	{
		lock_mutex(philo);
		if (access_sem(&philo->data->continue_sim, philo->data->continue_sem))
		{
			usleep(philo->data->ms_to_eat * 1000);
			sem_wait(philo->data->meal_count_sem);
			philo->meal_count++;
			philo->last_meal_ts = ts();
			sem_post(philo->data->meal_count_sem);
		}
		sem_post(philo->data->forks_sem);
		sem_post(philo->data->forks_sem);
		log_state(philo, 4);
		if (access_sem(&philo->data->continue_sim, philo->data->continue_sem))
			usleep(philo->data->ms_to_sleep * 1000);
	}
}

void	check_philosopher_state(t_data *data, long long current_ts)
{
	int	i;
	int	satiated;

	i = 0;
	satiated = 0;
	sem_wait(data->continue_sem);
	sem_wait(data->meal_count_sem);
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
	sem_post(data->meal_count_sem);
	if (data->times_eating && satiated >= data->philo_count)
	{
		data->continue_sim = 0;
		printf("%lli\tAll philosophers have had %i meals\n", \
			ms_elapsed(data), data->times_eating);
	}
	sem_post(data->continue_sem);
}

void	monitor_cycle(void *arg)
{
	t_data		*data;
	long long	current_ts;

	data = (t_data *)arg;
	while (access_sem(&data->continue_sim, data->continue_sem))
	{
		current_ts = ts();
		check_philosopher_state(data, current_ts);
		usleep(50);
	}
}
