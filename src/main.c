/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piuser <piuser@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:34:42 by louis.demet       #+#    #+#             */
/*   Updated: 2024/03/06 17:14:25 by piuser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_arguments(int argc)
{
	if (argc < 5 || argc > 6)
	{
		printf("args error: expected:\n"
			"1. (int) nb_philosophers\n"
			"2. (int) ms_to_starve\n"
			"3. (int) ms_to_eat\n"
			"4. (int) ms_to_sleep\n"
			"5. (int) times_eating (optional)\n");
		return (FAILURE);
	}
	return (SUCCESS);
}

int	create_processes(t_data *data)
{
	int		i;

	i = 0;
	while (i < data->philo_count)
	{
		data->philo[i]->pid = fork();
		if (data->philo[i]->pid < 0)
			return (ft_error(data, 3));
		if (data->philo[i]->pid == 0)
			philosopher_cycle((void *)data->philo[i]);
		i++;
	}
	return (SUCCESS);
}

void	kill_processes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
		kill(data->philo[i++]->pid, SIGKILL);
}

int	philosophers(t_data *data)
{
	if (!create_processes(data))
		return (FAILURE);
	sem_wait(data->continue_sem);
	kill_processes(data);
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	if (!check_arguments(argc))
		return (EXIT_FAILURE);
	data = malloc(sizeof(t_data));
	if (!data)
		return (ft_error(data, 1));
	if (!init_data(data, argv))
		return (EXIT_FAILURE);
	if (!philosophers(data))
		return (EXIT_FAILURE);
	free_data(data);
	return (EXIT_SUCCESS);
}
