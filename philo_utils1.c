/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:34:47 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/14 19:14:45 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_philo_stat(t_philo *philo, char *status)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->data->print_lock);
	if (!philo->data->stop)
	{
		pthread_mutex_lock(&philo->data->meal_lock);
		timestamp = current_time() - philo->data->start_time;
		pthread_mutex_unlock(&philo->data->meal_lock);
		printf("%lld %d %s\n", timestamp, philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->print_lock);
}

int	start_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				philosopher_routine, &data->philos[i]) != 0)
		{
			printf("Error: Failed to create philosopher thread %d\n", i + 1);
			return (1);
		}
		i++;
	}
	// Set start time after all threads are created
	pthread_mutex_lock(&data->meal_lock);
	data->start_time = current_time();
	pthread_mutex_unlock(&data->meal_lock);
	return (0);
}

void	cleanup(t_data *data)
{
	int	i;

	// Set stop flag to ensure all threads exit
	pthread_mutex_lock(&data->print_lock);
	data->stop = 1;
	pthread_mutex_unlock(&data->print_lock);
	
	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->meal_lock);
	free(data->forks);
	free(data->philos);
}