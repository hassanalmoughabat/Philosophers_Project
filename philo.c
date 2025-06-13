#include "philo.h"

int init_data(t_data *data, int argc, char **argv)
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
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (1); 
	for (int i = 0; i < data->num_philos; i++)
		pthread_mutex_init(&data->forks[i], NULL);

	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->meal_lock, NULL);

	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (1);

	int i = 0;
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

	return (0);
}

long long current_time(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int check_args(int argc, char **argv)
{
	if (argc< 5 || argc > 6)
	{
		printf(
			"Error: Invalid number of arguments.\n"
			"Usage: ./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_meals]\n"
			"Example: ./philo 5 800 200 200 3\n"
			"Note: The last argument is optional and specifies the number of meals each philosopher should eat.\n"
			"       If not provided, philosophers will eat indefinitely until a philosopher dies.\n"
		);
		return (1);
	}
	if (atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0 || atoi(argv[3]) <= 0 || atoi(argv[4]) <= 0)
	{
		printf("Error: Invalid argument values. All arguments must be positive integers.\n");
		return (1);
	}
	return (0);
}

int main(int argc, char **argv)
{
	t_philo *philos;
	t_data data;
	if (check_args(argc, argv) != 0)
	{
		return (1);
	}
	if (init_data(&data, argc, argv) != 0)
	{
		printf("Error: Failed to initialize data.\n");
		return (1);
	}
	if (start_philosophers(&data) != 0)
	{
		printf("Error: Failed to start philosopher threads.\n");
		cleanup(&data);
		return (1);
	}
	check_philospher_death(&data);
	cleanup(&data);
	return (0);
}