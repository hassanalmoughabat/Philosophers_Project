/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 17:00:16 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/14 19:32:14 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_all_ate(t_data *data)
{
	int	i;
	int	all_ate;

	if (data->num_meals == -1)
		return (0);
	all_ate = 1;
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->meal_lock);
		if (data->philos[i].meals_eaten < data->num_meals)
			all_ate = 0;
		pthread_mutex_unlock(&data->meal_lock);
		if (!all_ate)
			break;
		i++;
	}
	return (all_ate);
}

static int	philosopher_died(t_data *data, int i)
{
	long long	death_time;

	pthread_mutex_lock(&data->print_lock);
	if (!data->stop)
	{
		data->stop = 1;
		pthread_mutex_lock(&data->meal_lock);
		death_time = current_time() - data->start_time;
		pthread_mutex_unlock(&data->meal_lock);
		printf("%lld %d died\n", death_time, data->philos[i].id);
	}
	pthread_mutex_unlock(&data->print_lock);
	return (1);
}

static int	check_death(t_data *data)
{
	int			i;
	long long	time_since_meal;
	long long	current;
	long long	start;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->meal_lock);
		start = data->start_time;
		current = current_time() - start;
		time_since_meal = current - data->philos[i].last_meal;
		pthread_mutex_unlock(&data->meal_lock);
		
		if (time_since_meal > data->time_to_die)
			return (philosopher_died(data, i));
		i++;
	}
	return (0);
}

int check_philospher_death(t_data *data)
{
	while (1)
	{
		if (check_all_ate(data))
		{
			pthread_mutex_lock(&data->print_lock);
			data->stop = 1;
			pthread_mutex_unlock(&data->print_lock);
			return (0);
		}
		if (check_death(data))
			return (1);
		usleep(1000);
	}
}