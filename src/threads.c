/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruiferna <ruiferna@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 10:26:12 by ruiferna          #+#    #+#             */
/*   Updated: 2025/10/14 10:51:47 by ruiferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	allocate_threads(t_shared *shared, pthread_t **threads)
{
	*threads = malloc(sizeof(pthread_t) * shared->num_philos);
	if (!*threads)
		return (0);
	return (1);
}

void	start_philosophers(t_shared *shared, pthread_t *threads,
		pthread_t *monitor)
{
	int	i;

	i = 0;
	while (i < shared->num_philos)
	{
		pthread_create(&threads[i], NULL, philo_routine, &shared->philos[i]);
		i++;
	}
	pthread_create(monitor, NULL, monitor_routine, shared);
}

void	join_philosophers(t_shared *shared, pthread_t *threads,
		pthread_t monitor)
{
	int	i;

	i = 0;
	while (i < shared->num_philos)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	pthread_join(monitor, NULL);
}
