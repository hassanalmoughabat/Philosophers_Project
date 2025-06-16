/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 16:59:47 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/16 20:33:23 by hal-moug         ###   ########.fr       */
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

void	take_forks(t_philo *philo)
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

void	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
}

void	philo_eat(t_philo *philo)
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
			break ;
		usleep(100);
	}
}

void	philo_sleep(t_philo *philo)
{
	long long	sleep_start;

	print_philo_stat(philo, "is sleeping");
	sleep_start = current_time();
	while (current_time() - sleep_start < philo->data->time_to_sleep)
	{
		if (check_death(philo))
			break ;
		usleep(100);
	}
}
