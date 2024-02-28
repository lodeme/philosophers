/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louis.demetz <louis.demetz@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 09:42:56 by louis.demet       #+#    #+#             */
/*   Updated: 2024/02/28 20:30:00 by louis.demet      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

# define NUM_PHILOSOPHERS 2
# define SUCCESS 1
# define FAILURE 0

typedef struct s_data	t_data;

typedef struct s_philo
{
	int			id;
	long long	last_meal_ts;
	int			meal_count;
	t_data		*data;
}	t_philo;

typedef struct s_data
{
	int				philo_count;
	int				ms_to_starve;
	int				ms_to_eat;
	int				ms_to_sleep;
	int				times_eating;
	int				sim_ok;
	long long		ts_start;
	pthread_t		*thread;
	pthread_mutex_t	*mutex;
	t_philo			**philo;
}	t_data;

//PHILO
int			philosophers(t_data *data);

// INIT
int			init_data(t_data *data, char **argv);

// UTILS
int			ft_atoi(const char *str);
long long	ts(void);
long long	ms_elapsed(t_data *data);

// FREE
void		free_data(t_data *data);
int			ft_error(t_data *data, int err);

#endif