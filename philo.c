/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hal-moug <hal-moug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:40:06 by hal-moug          #+#    #+#             */
/*   Updated: 2025/06/14 19:09:49 by hal-moug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	check_args(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc < 5 || argc > 6)
	{
		printf("Error: Wrong number of arguments\n");
		printf("Usage: ./philo number_of_philosophers time_to_die ");
		printf("time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
			{
				printf("Error: Arguments must be positive integers\n");
				return (1);
			}
			j++;
		}
		if (atoi(argv[i]) <= 0 || atoi(argv[i]) > 2147483647)
		{
			printf("Error: Arguments must be positive integers\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (check_args(argc, argv) != 0)
		return (1);
	if (init_data(&data, argc, argv) != 0)
	{
		printf("Error: Failed to initialize data.\n");
		return (1);
	}
	if (start_philosophers(&data) != 0)
	{
		cleanup(&data);
		return (1);
	}
	check_philospher_death(&data);
	cleanup(&data);
	return (0);
}