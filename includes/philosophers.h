/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lodemetz <lodemetz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 09:42:56 by louis.demet       #+#    #+#             */
/*   Updated: 2024/02/29 15:01:47 by lodemetz         ###   ########.fr       */
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
	int						id;
	volatile long long		last_meal_ts;
	volatile int			meal_count;
	t_data					*data;
}	t_philo;

typedef struct s_data
{
	int				philo_count;
	int				ms_to_starve;
	int				ms_to_eat;
	int				ms_to_sleep;
	int				times_eating;
	volatile int	continue_sim;
	long long		ts_start;
	pthread_t		*thread;
	pthread_mutex_t	*mutex;
	t_philo			**philo;
}	t_data;

// THREADS
int			philosophers(t_data *data);

// CYCLES
void		*philosopher_cycle(void *arg);
void		*monitor_cycle(void *arg);

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