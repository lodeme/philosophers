/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piuser <piuser@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:34:42 by louis.demet       #+#    #+#             */
/*   Updated: 2024/03/04 00:21:53 by piuser           ###   ########.fr       */
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
	return (EXIT_SUCCESS);
}
