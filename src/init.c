/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piuser <piuser@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:58:33 by louis.demet       #+#    #+#             */
/*   Updated: 2024/03/06 10:12:32 by piuser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	init_parameters(t_data *data, char **argv)
{
	data->philo_count = ft_atoi(argv[1]);
	data->ms_to_starve = ft_atoi(argv[2]);
	data->ms_to_eat = ft_atoi(argv[3]);
	data->ms_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		data->times_eating = ft_atoi(argv[5]);
	else
		data->times_eating = 0;
	data->ts_start = ts();
	data->continue_sim = 1;
	data->philo = 0;
	data->continue_sem = 0;
	data->meal_count_sem = 0;
	data->forks_sem = 0;
}

int	init_semaphores(t_data *data)
{
	sem_unlink("forks_sem");
	sem_unlink("meal_count_sem");
	sem_unlink("continue_sem");
	data->forks_sem = sem_open("forks_sem", O_CREAT, 0644, data->philo_count);
	data->meal_count_sem = sem_open("meal_count_sem", O_CREAT, 0644, 1);
	data->continue_sem = sem_open("continue_sem", O_CREAT, 0644, 1);
	if (!data->forks_sem || !data->meal_count_sem || !data->continue_sem)
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
		i++;
	}
	return (SUCCESS);
}

int	init_data(t_data *data, char **argv)
{
	init_parameters(data, argv);
	if (!init_semaphores(data))
		return (FAILURE);
	if (!init_philosophers(data))
		return (FAILURE);
	return (SUCCESS);
}
