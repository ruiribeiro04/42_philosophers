#include "philo.h"

int	parse_arguments(int argc, char **argv, t_shared *shared)
{
	if (argc != 5 && argc != 6)
	{
		printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_must_eat]\n",
			argv[0]);
		return (0);
	}
	if (!is_valid_number(argv[1]) || !is_valid_number(argv[2])
		|| !is_valid_number(argv[3]) || !is_valid_number(argv[4]))
		return (0);
	shared->num_philos = ft_atoi(argv[1]);
	shared->time_to_die = ft_atoi(argv[2]);
	shared->time_to_eat = ft_atoi(argv[3]);
	shared->time_to_sleep = ft_atoi(argv[4]);
	if (shared->num_philos <= 0 || shared->time_to_die <= 0
		|| shared->time_to_eat <= 0 || shared->time_to_sleep <= 0)
		return (0);
	if (argc == 6)
	{
		if (!is_valid_number(argv[5]))
			return (0);
		shared->must_eat_count = ft_atoi(argv[5]);
		if (shared->must_eat_count <= 0)
			return (0);
		shared->has_must_eat = 1;
	}
	else
		shared->has_must_eat = 0;
	return (1);
}

void	cleanup(t_shared *shared, pthread_t *threads)
{
	int	i;

	i = 0;
	while (i < shared->num_philos)
	{
		pthread_mutex_destroy(&shared->forks[i]);
		pthread_mutex_destroy(&shared->philos[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&shared->printf_mutex);
	pthread_mutex_destroy(&shared->stop_mutex);
	free(shared->forks);
	free(shared->philos);
	free(threads);
}

int	main(int argc, char **argv)
{
	t_shared	shared;
	pthread_t	*threads;
	pthread_t	monitor;
	int			i;

	memset(&shared, 0, sizeof(t_shared));
	if (!parse_arguments(argc, argv, &shared))
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}
	if (!init_shared(&shared))
	{
		printf("Error: Initialization failed\n");
		return (1);
	}
	init_philosophers(&shared);
	threads = malloc(sizeof(pthread_t) * shared.num_philos);
	if (!threads)
	{
		cleanup(&shared, NULL);
		return (1);
	}
	i = 0;
	while (i < shared.num_philos)
	{
		pthread_create(&threads[i], NULL, philo_routine, &shared.philos[i]);
		i++;
	}
	pthread_create(&monitor, NULL, monitor_routine, &shared);
	i = 0;
	while (i < shared.num_philos)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	cleanup(&shared, threads);
	return (0);
}
