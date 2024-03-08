/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycles.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lodemetz <lodemetz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 13:00:53 by lodemetz          #+#    #+#             */
/*   Updated: 2024/03/08 19:23:45 by lodemetz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	log_state(t_philo *philo, int step)
{
	int	id;

	id = philo->id;
	sem_wait(philo->data->message_sem);
	if (step == THINKING)
		printf("%lli\t%d is thinking\n", ms_elapsed(philo->data), id);
	else if (step == FORK)
		printf("%lli\t%d has taken a fork\n", ms_elapsed(philo->data), id);
	else if (step == EATING)
		printf("%lli\t%d is eating\n", ms_elapsed(philo->data), id);
	else if (step == SLEEPING)
		printf("%lli\t%d is sleeping\n", ms_elapsed(philo->data), id);
	sem_post(philo->data->message_sem);
}

void	*check_meals(void *arg)
{
	int		i;
	t_data	*data;

	data = (t_data *)arg;
	i = 0;
	while (i < data->philo_count)
	{
		sem_wait(data->meals_sem);
		i++;
	}
	sem_wait(data->message_sem);
	printf("%lli\tAll philosophers had %i meals\n", ms_elapsed(data), \
		data->times_eating);
	sem_post(data->continue_sem);
	return (0);
}

void	*check_end(void *arg)
{
	t_philo	*philo;

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
		if (philo->meal_count >= philo->data->times_eating && \
			philo->data->times_eating)
		{
			sem_post(philo->data->meals_sem);
			if (philo->pid > 0)
				kill(philo->pid, SIGKILL);
			sem_post(philo->data->message_sem);
			break ;
		}
		sem_post(philo->data->message_sem);
		usleep(50);
	}
	return (0);
}

void	do_routine(t_philo *philo)
{
	log_state(philo, THINKING);
	if (philo->meal_count > 0)
		usleep(100);
	sem_wait(philo->data->forks_sem);
	log_state(philo, FORK);
	sem_wait(philo->data->forks_sem);
	log_state(philo, FORK);
	log_state(philo, EATING);
	philo->last_meal_ts = ts();
	usleep(philo->data->ms_to_eat * 1000);
	sem_post(philo->data->forks_sem);
	sem_post(philo->data->forks_sem);
	log_state(philo, SLEEPING);
	philo->meal_count++;
	usleep(philo->data->ms_to_sleep * 1000);
}

void	philosopher_cycle(void *arg)
{
	t_philo		*philo;
	pthread_t	thread;

	if (pthread_create(&thread, NULL, check_end, arg))
		return ;
	pthread_detach(thread);
	philo = (t_philo *)arg;
	if (philo->id % 2 == 1)
		usleep(philo->data->ms_to_eat * 1000);
	while (1)
		do_routine(philo);
}
