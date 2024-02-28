/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis.demetz <louis.demetz@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:34:42 by louis.demet       #+#    #+#             */
/*   Updated: 2024/02/28 20:14:43 by louis.demet      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_arguments(int argc)
{
	if (argc < 5 || argc > 6)
	{
	printf("args error: expected:\n"
		"- nb_philosophers\n"
		"- ms_to_starve\n"
		"- ms_to_eat\n"
		"- ms_to_sleep\n"
		"- times_eating (optional)\n");
		return (FAILURE);
	}
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
	init_data(data, argv);
	if (!data)
		return (EXIT_FAILURE);
	if (!philosophers(data))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
