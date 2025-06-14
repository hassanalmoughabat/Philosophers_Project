/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_sctions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 16:59:47 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/14 16:59:53 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

static void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
	print_philo_stat(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
	print_philo_stat(philo, "has taken a fork");
}

static void	eat_and_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->meal_lock);
	philo->last_meal = current_time() - philo->data->start_time;
	pthread_mutex_unlock(&philo->data->meal_lock);
	print_philo_stat(philo, "is eating");
	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
	pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
	print_philo_stat(philo, "is sleeping");
	usleep(philo->data->time_to_sleep * 1000);
	print_philo_stat(philo, "is thinking");
}

static int	check_meal_count(t_philo *philo)
{
	if (philo->data->num_meals == -1)
		return (0);
	pthread_mutex_lock(&philo->data->meal_lock);
	philo->meals_eaten++;
	if (philo->meals_eaten >= philo->data->num_meals)
	{
		pthread_mutex_unlock(&philo->data->meal_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->meal_lock);
	return (0);
}

static void	*handle_single_philo(t_philo *philo)
{
	print_philo_stat(philo, "has taken a fork");
	usleep(philo->data->time_to_die * 1000);
	return (NULL);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->start_time == 0)
		usleep(1000);
	if (philo->data->num_philos == 1)
		return (handle_single_philo(philo));
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		take_forks(philo);
		eat_and_sleep(philo);
		if (check_meal_count(philo))
			break ;
	}
	return (NULL);
}