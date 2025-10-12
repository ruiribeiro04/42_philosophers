/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruiferna <ruiferna@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 02:10:41 by ruiferna          #+#    #+#             */
/*   Updated: 2025/10/12 02:10:42 by ruiferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_shared(t_shared *shared)
{
	int	i;

	shared->simulation_stop = 0;
	shared->forks = malloc(sizeof(pthread_mutex_t) * shared->num_philos);
	if (!shared->forks)
		return (0);
	shared->philos = malloc(sizeof(t_philo) * shared->num_philos);
	if (!shared->philos)
	{
		free(shared->forks);
		return (0);
	}
	i = 0;
	while (i < shared->num_philos)
	{
		pthread_mutex_init(&shared->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&shared->printf_mutex, NULL);
	pthread_mutex_init(&shared->stop_mutex, NULL);
	return (1);
}

void	init_philosophers(t_shared *shared)
{
	int			i;
	long long	current_time;

	current_time = get_current_time();
	shared->start_time = current_time;
	i = 0;
	while (i < shared->num_philos)
	{
		shared->philos[i].id = i + 1;
		shared->philos[i].meals_eaten = 0;
		shared->philos[i].last_meal_time = current_time;
		shared->philos[i].shared = shared;
		pthread_mutex_init(&shared->philos[i].meal_mutex, NULL);
		i++;
	}
}
