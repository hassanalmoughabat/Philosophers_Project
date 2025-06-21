/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 18:22:17 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/16 20:33:04 by hal-moug         ###   ########.fr       */
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

static int	philo_life_cycle(t_philo *philo)
{
	take_forks(philo);
	if (check_death(philo))
	{
		put_forks(philo);
		return (1);
	}
	philo_eat(philo);
	put_forks(philo);
	if (check_meal_count(philo) || check_death(philo))
		return (1);
	philo_sleep(philo);
	if (check_death(philo))
		return (1);
	print_philo_stat(philo, "is thinking");
	usleep(100);
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
		if (philo_life_cycle(philo))
			break ;
	}
	return (NULL);
}

void	*handle_single_philo(t_philo *philo)
{
	print_philo_stat(philo, "has taken a fork");
	while (!check_death(philo))
		usleep(100);
	return (NULL);
}

int	wait_for_start(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(&philo->data->meal_lock);
		if (philo->data->start_time != 0)
		{
			pthread_mutex_unlock(&philo->data->meal_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->data->meal_lock);
		usleep(100);
	}
	return (0);
}
