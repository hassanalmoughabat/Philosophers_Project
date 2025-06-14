/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 16:59:47 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/14 19:33:27 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_death(t_philo *philo)
{
	int	ret;

	pthread_mutex_lock(&philo->data->print_lock);
	ret = philo->data->stop;
	pthread_mutex_unlock(&philo->data->print_lock);
	return (ret);
}

static void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
		print_philo_stat(philo, "has taken a fork");
		pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
		print_philo_stat(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
		print_philo_stat(philo, "has taken a fork");
		pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
		print_philo_stat(philo, "has taken a fork");
	}
}

static void	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
}

static void	philo_eat(t_philo *philo)
{
	long long	eat_start;

	pthread_mutex_lock(&philo->data->meal_lock);
	philo->last_meal = current_time() - philo->data->start_time;
	pthread_mutex_unlock(&philo->data->meal_lock);
	
	print_philo_stat(philo, "is eating");
	eat_start = current_time();
	
	while (current_time() - eat_start < philo->data->time_to_eat)
	{
		if (check_death(philo))
			break;
		usleep(100);
	}
}

static void	philo_sleep(t_philo *philo)
{
	long long	sleep_start;

	print_philo_stat(philo, "is sleeping");
	sleep_start = current_time();
	
	while (current_time() - sleep_start < philo->data->time_to_sleep)
	{
		if (check_death(philo))
			break;
		usleep(100);
	}
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
	while (!check_death(philo))
		usleep(100);
	return (NULL);
}

static int	wait_for_start(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(&philo->data->meal_lock);
		if (philo->data->start_time != 0)
		{
			pthread_mutex_unlock(&philo->data->meal_lock);
			break;
		}
		pthread_mutex_unlock(&philo->data->meal_lock);
		usleep(100);
	}
	return (0);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	
	wait_for_start(philo);
	pthread_mutex_lock(&philo->data->meal_lock);
	philo->last_meal = 0;
	pthread_mutex_unlock(&philo->data->meal_lock);
	
	if (philo->data->num_philos == 1)
		return (handle_single_philo(philo));
	if (philo->id % 2 == 0)
		usleep(1000);
		
	while (!check_death(philo))
	{
		take_forks(philo);
		if (check_death(philo))
		{
			put_forks(philo);
			break;
		}
		philo_eat(philo);
		put_forks(philo);
		if (check_meal_count(philo) || check_death(philo))
			break;
		philo_sleep(philo);
		if (check_death(philo))
			break;
		print_philo_stat(philo, "is thinking");
		usleep(100);
	}
	return (NULL);
}