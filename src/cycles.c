/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycles.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piuser <piuser@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 13:00:53 by lodemetz          #+#    #+#             */
/*   Updated: 2024/03/07 11:50:18 by piuser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void log_state(t_philo *philo, int step)
{
	int id;

	id = philo->id;
	sem_wait(philo->data->message_sem);
	if (step == THINKING)
		printf("%lli\t%d is thinking.\n", ms_elapsed(philo->data), id);
	else if (step == FORK)
		printf("%lli\t%d has taken a fork.\n", ms_elapsed(philo->data), id);
	else if (step == EATING)
		printf("%lli\t%d is eating.\n", ms_elapsed(philo->data), id);
	else if (step == SLEEPING)
		printf("%lli\t%d is sleeping.\n", ms_elapsed(philo->data), id);
	sem_post(philo->data->message_sem);
}

void *check_end(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->data->message_sem);
		if ((ts() - philo->last_meal_ts) > (philo->data->ms_to_starve))
		{
			printf("%lli\t%i died\n", ms_elapsed(philo->data), philo->id);
			sem_post(philo->data->continue_sem);
			break ;
		}
		if (philo->meal_count >= philo->data->times_eating)
		{
			sem_post(philo->data->meals_sem);
			if (philo->pid > 0)
				kill(philo->pid, SIGKILL);
		}
		sem_post(philo->data->message_sem);
		usleep(50);
	}
	return (0);
}

void philosopher_cycle(void *arg)
{
	t_philo *philo;
	pthread_t thread;

	pthread_create(&thread, NULL, check_end, arg);
	pthread_detach(thread);
	philo = (t_philo *)arg;
	if (philo->id % 2 == 1)
		usleep(philo->data->ms_to_eat * 1000);
	while (1)
	{
		log_state(philo, THINKING);
		sem_wait(philo->data->forks_sem);
		log_state(philo, FORK);
		sem_wait(philo->data->forks_sem);
		log_state(philo, FORK);
		log_state(philo, EATING);
		usleep(philo->data->ms_to_eat * 1000);
		philo->meal_count++;
		philo->last_meal_ts = ts();
		sem_post(philo->data->forks_sem);
		sem_post(philo->data->forks_sem);
		log_state(philo, SLEEPING);
		usleep(philo->data->ms_to_sleep * 1000);
	}
}
