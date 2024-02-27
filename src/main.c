/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis.demetz <louis.demetz@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:34:42 by louis.demet       #+#    #+#             */
/*   Updated: 2024/02/28 00:49:07 by louis.demet      ###   ########.fr       */
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

int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_create(&data->thread[i], NULL, philosopher_lifecycle, (void *)&data->philo[i]))
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
	create_threads(data);
	join_threads(data);
	destroy_mutexes(data);
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
	data = malloc(sizeof(t_data));
	if (!data)
		return (ft_error(data, 1));
	init_data(data, argv);
	if (!data)
		return (EXIT_FAILURE);
	if (!philosophers(data))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
