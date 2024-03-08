/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lodemetz <lodemetz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:58:33 by louis.demet       #+#    #+#             */
/*   Updated: 2024/03/08 15:04:46 by lodemetz         ###   ########.fr       */
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
	data->philo = 0;
	data->continue_sem = 0;
	data->message_sem = 0;
	data->meals_sem = 0;
	data->forks_sem = 0;
	return (SUCCESS);
}

int	init_semaphores(t_data *data)
{
	sem_unlink("forks_sem");
	sem_unlink("continue_sem");
	sem_unlink("message_sem");
	sem_unlink("meals_sem");
	data->forks_sem = sem_open("forks_sem", O_CREAT, 0644, data->philo_count);
	data->continue_sem = sem_open("continue_sem", O_CREAT, 0644, 0);
	data->message_sem = sem_open("message_sem", O_CREAT, 0644, 1);
	data->meals_sem = sem_open("meals_sem", O_CREAT, 0644, 0);
	if (!data->forks_sem || !data->message_sem || !data->continue_sem
		|| !data->meals_sem)
		return (ft_error(data, 2));
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
		data->philo[i]->pid = 0;
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
	if (!init_semaphores(data))
		return (FAILURE);
	if (!init_philosophers(data))
		return (FAILURE);
	return (SUCCESS);
}
