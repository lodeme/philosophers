/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: piuser <piuser@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 09:42:56 by louis.demet       #+#    #+#             */
/*   Updated: 2024/03/07 11:12:09 by piuser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <semaphore.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <pthread.h>
# include <signal.h>

# define SUCCESS 1
# define FAILURE 0
# define THINKING 1
# define FORK 2
# define EATING 3
# define SLEEPING 4

typedef struct s_data	t_data;

typedef struct s_philo
{
	int			id;
	long long	last_meal_ts;
	int			meal_count;
	pid_t		pid;
	t_data		*data;
}	t_philo;

typedef struct s_data
{
	int			philo_count;
	int			ms_to_starve;
	int			ms_to_eat;
	int			ms_to_sleep;
	int			times_eating;
	sem_t		*continue_sem;
	sem_t		*forks_sem;
	sem_t		*message_sem;
	sem_t		*meals_sem;
	long long	ts_start;
	t_philo		**philo;
}	t_data;

// THREADS
int			philosophers(t_data *data);

// CYCLES
void		philosopher_cycle(void *arg);

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