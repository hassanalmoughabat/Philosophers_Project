/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 17:00:16 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/14 17:00:23 by hal-moug         ###   ########.fr       */
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
		{
			all_ate = 0;
			pthread_mutex_unlock(&data->meal_lock);
			break ;
		}
		pthread_mutex_unlock(&data->meal_lock);
		i++;
	}
	return (all_ate);
}

static int	check_death(t_data *data, long long current)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->meal_lock);
		if (current - data->philos[i].last_meal > data->time_to_die)
		{
			pthread_mutex_unlock(&data->meal_lock);
			pthread_mutex_lock(&data->print_lock);
			printf("%lld %d died\n", current, data->philos[i].id);
			pthread_mutex_unlock(&data->print_lock);
			return (1);
		}
		pthread_mutex_unlock(&data->meal_lock);
		i++;
	}
	return (0);
}

int	check_philospher_death(t_data *data)
{
	long long	current;

	while (1)
	{
		if (check_all_ate(data))
			return (0);
		current = current_time() - data->start_time;
		if (check_death(data, current))
			return (1);
		usleep(1000);
	}
}