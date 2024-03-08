/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piuser <piuser@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:58:33 by louis.demet       #+#    #+#             */
/*   Updated: 2024/03/08 09:27:55 by piuser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_parameters(t_data *data)
{
	if (data->philo_count < 1
		|| data->ms_to_starve < 1
		|| data->ms_to_eat < 1
		|| data->ms_to_sleep < 1
		|| data->times_eating < 0)
		return (ft_error(data, 4));
	return (SUCCESS);
}

int	init_parameters(t_data *data, char **argv)
{
	data->philo_count = ft_atoi(argv[1]);
	data->ms_to_starve = ft_atoi(argv[2]);
	data->ms_to_eat = ft_atoi(argv[3]);
	data->ms_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
	{
		if (ft_atoi(argv[5]) == 0)
			return (ft_error(data, 4));
		data->times_eating = ft_atoi(argv[5]);
	}
	else
		data->times_eating = 0;
	data->ts_start = ts();
	data->thread = 0;
	data->mutex = 0;
	data->philo = 0;
	data->continue_sim = 1;
	return (SUCCESS);
}

int	init_mutexes(t_data *data)
{
	int	i;

	i = 0;
	data->mutex = malloc(sizeof(pthread_mutex_t) * data->philo_count);
	if (!data->mutex)
		return (ft_error(data, 1));
	while (i < data->philo_count)
	{
		if (pthread_mutex_init(&data->mutex[i++], NULL) != 0)
			return (ft_error(data, 2));
	}
	data->continue_mut = malloc(sizeof(pthread_mutex_t));
	if (!data->continue_mut)
		return (ft_error(data, 2));
	pthread_mutex_init(data->continue_mut, NULL);
	data->meal_count_mutex = malloc(sizeof(pthread_mutex_t));
	if (!data->meal_count_mutex)
		return (ft_error(data, 2));
	pthread_mutex_init(data->meal_count_mutex, NULL);
	return (SUCCESS);
}

int	init_philosophers(t_data *data)
{
	int	i;

	i = 0;
	data->philo = malloc(sizeof(t_philo *) * data->philo_count);
	if (!data->philo)
		return (ft_error(data, 1));
	while (i < data->philo_count)
	{
		data->philo[i] = malloc(sizeof(t_philo));
		if (!data->philo[i])
			return (ft_error(data, 1));
		data->philo[i]->id = i;
		data->philo[i]->last_meal_ts = data->ts_start;
		data->philo[i]->data = data;
		data->philo[i]->meal_count = 0;
		i++;
	}
	return (SUCCESS);
}

int	init_data(t_data *data, char **argv)
{
	if (!init_parameters(data, argv))
		return (FAILURE);
	if (!check_parameters(data))
		return (FAILURE);
	data->thread = malloc(sizeof(pthread_t) * (data->philo_count + 1));
	if (!data->thread)
		return (ft_error(data, 1));
	if (!init_mutexes(data))
		return (FAILURE);
	if (!init_philosophers(data))
		return (FAILURE);
	return (SUCCESS);
}
