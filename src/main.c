/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruiferna <ruiferna@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 02:10:44 by ruiferna          #+#    #+#             */
/*   Updated: 2025/10/15 12:05:45 by ruiferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_shared	shared;
	pthread_t	*threads;
	pthread_t	monitor;

	memset(&shared, 0, sizeof(t_shared));
	if (!ft_parse_arguments(argc, argv, &shared))
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}
	if (!ft_init_shared(&shared))
	{
		printf("Error: Initialization failed\n");
		return (1);
	}
	ft_init_philosophers(&shared);
	if (!ft_allocate_threads(&shared, &threads))
	{
		ft_cleanup(&shared, NULL);
		return (1);
	}
	ft_start_philosophers(&shared, threads, &monitor);
	ft_join_philosophers(&shared, threads, monitor);
	ft_cleanup(&shared, threads);
	return (0);
}

void	ft_cleanup(t_shared *shared, pthread_t *threads)
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
