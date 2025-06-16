/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 16:00:00 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/16 18:34:30 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_meals;
	int				stop;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	meal_lock;
	t_philo			*philos;
}	t_data;

typedef struct s_philo
{
	int			id;
	long long	last_meal;
	pthread_t	thread;
	int			meals_eaten;
	int			right_fork;
	int			left_fork;
	t_data		*data;
}	t_philo;

// philo.c
int			check_args(int argc, char **argv);
long long	current_time(void);

// philo_init.c
int			init_data(t_data *data, int argc, char **argv);

// philo_utils1.c
void		print_philo_stat(t_philo *philo, char *status);
int			start_philosophers(t_data *data);
void		cleanup(t_data *data);

// philo_actions.c
void		*philosopher_routine(void *arg);
int			wait_for_start(t_philo *philo);
void		*handle_single_philo(t_philo *philo);
int			check_meal_count(t_philo *philo);
void		philo_sleep(t_philo *philo);
void		philo_eat(t_philo *philo);
void		put_forks(t_philo *philo);
void		take_forks(t_philo *philo);
// int			check_death(t_philo *philo);
// philo_monitor.c
int			check_philospher_death(t_data *data);

#endif