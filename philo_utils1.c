/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:34:47 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/13 20:52:52 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void print_philo_stat(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->data->print_lock);
	printf("%lld %d %s\n", current_time() - philo->data->start_time, philo->id, status);
	pthread_mutex_unlock(&philo->data->print_lock);
}

void *philosopher_routine(void *arg)
{
	t_philo *philo;
	t_data *data;
	
	if (data->start_time == 0)
		usleep(1000);
	if (data->num_philos == 1)
	{
		print_philo_stat(philo, "has taken a fork");
		usleep(data->time_to_die * 1000);
		return NULL;
	}
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		pthread_mutex_lock(&data->forks[philo->left_fork]);
		print_philo_stat(philo, "has taken a fork");
		pthread_mutex_lock(&data->forks[philo->right_fork]);
		print_philo_stat(philo, "has taken a fork");
		pthread_mutex_lock(&data->meal_lock);
		philo->last_meal = current_time() - data->start_time;
		pthread_mutex_unlock(&data->meal_lock);
		print_philo_stat(philo, "is eating");
		usleep(data->time_to_eat * 1000);
		
		pthread_mutex_unlock(&data->forks[philo->right_fork]);
		pthread_mutex_unlock(&data->forks[philo->left_fork]);
		
		if (data->num_meals != -1)
		{
			pthread_mutex_lock(&data->meal_lock);
			philo->meals_eaten++;
			if (philo->meals_eaten >= data->num_meals)
			{
				pthread_mutex_unlock(&data->meal_lock);
				break;
			}
			pthread_mutex_unlock(&data->meal_lock);
		}
		print_philo_stat(philo, "is sleeping");
		usleep(data->time_to_sleep * 1000);
		print_philo_stat(philo, "is thinking");
	}
	return NULL;
}

int check_philospher_death(t_data *data)
{
	int i;
	long long current;
	
	while (1)
	{
		       if (data->num_meals != -1)
        {
            i = 0;
            while (i < data->num_philos)
            {
                pthread_mutex_lock(&data->meal_lock);
                if (data->philos[i].meals_eaten < data->num_meals)
                {
                    pthread_mutex_unlock(&data->meal_lock);
                    break;
                }
                pthread_mutex_unlock(&data->meal_lock);
                i++;
            }
            if (i == data->num_philos)
                return (0); 
        }
	}
	
	 i = 0;
        current = current_time() - data->start_time;
        while (i < data->num_philos)
        {
            pthread_mutex_lock(&data->meal_lock);
            if (current - data->philos[i].last_meal > data->time_to_die)
            {
                pthread_mutex_lock(&data->print_lock);
                printf("%lld %d died\n", current, data->philos[i].id);
                return (1); // A philosopher has died
            }
            pthread_mutex_unlock(&data->meal_lock);
            i++;
            usleep(100); // Small delay to avoid high CPU usage
        }
}

int start_philosophers(t_data *data)
{
	int i;

	data->start_time = current_time();
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philosopher_routine, &data->philos[i]) != 0)
		{
			printf("Error: Failed to create philosopher thread %d\n", i + 1);
			return (1);
		}
		i++;
	}
	return (0);
}

void cleanup(t_data *data)
{
	int i;
    
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
