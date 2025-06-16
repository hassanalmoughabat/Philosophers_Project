/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 17:00:43 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/16 18:21:00 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->meal_lock, NULL);
	return (0);
}

static void	init_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].last_meal = 0;
		data->philos[i].meals_eaten = 0;
		data->philos[i].right_fork = i;
		data->philos[i].left_fork = (i + 1) % data->num_philos;
		data->philos[i].data = data;
		i++;
	}
}

int	init_data(t_data *data, int argc, char **argv)
{
	data->num_philos = atoi(argv[1]);
	data->time_to_die = atoi(argv[2]);
	data->time_to_eat = atoi(argv[3]);
	data->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		data->num_meals = atoi(argv[5]);
	else
		data->num_meals = -1;
	data->start_time = 0;
	data->stop = 0;
	if (init_mutexes(data) != 0)
		return (1);
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
	{
		free(data->forks);
		return (1);
	}
	init_philosophers(data);
	return (0);
}
