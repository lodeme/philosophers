/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piuser <piuser@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 20:34:42 by louis.demet       #+#    #+#             */
/*   Updated: 2024/03/04 00:17:19 by piuser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int g_err_code = 0;

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
	{
		ft_error(data, 1);
		return (g_err_code);
	}
	if (!init_data(data, argv))
		return (g_err_code);
	if (!philosophers(data))
		return (g_err_code);
	if (g_err_code != EXIT_SUCCESS)
		return (g_err_code);
	return (EXIT_SUCCESS);
}
