/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruiferna <ruiferna@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 02:10:32 by ruiferna          #+#    #+#             */
/*   Updated: 2025/10/15 14:15:02 by ruiferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_take_forks(t_philo *philo, int *first, int *second)
{
	int	left_fork;
	int	right_fork;

	left_fork = philo->id - 1;
	right_fork = philo->id % philo->shared->num_philos;
	*first = left_fork;
	*second = right_fork;
	if (*first > *second)
	{
		*first = right_fork;
		*second = left_fork;
	}
	pthread_mutex_lock(&philo->shared->forks[*first]);
	ft_print_message(philo->shared, philo->id, "has taken a fork");
	if (philo->shared->num_philos == 1)
	{
		pthread_mutex_unlock(&philo->shared->forks[*first]);
		return (0);
	}
	pthread_mutex_lock(&philo->shared->forks[*second]);
	ft_print_message(philo->shared, philo->id, "has taken a fork");
	return (1);
}

void	ft_philo_eat(t_philo *philo)
{
	int	first;
	int	second;

	if (!ft_take_forks(philo, &first, &second))
		return ;
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = ft_get_current_time();
	pthread_mutex_unlock(&philo->meal_mutex);
	ft_print_message(philo->shared, philo->id, "is eating");
	ft_precise_sleep(philo->shared->time_to_eat);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	pthread_mutex_unlock(&philo->shared->forks[second]);
	pthread_mutex_unlock(&philo->shared->forks[first]);
}

void	*ft_philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		ft_precise_sleep(50);
	while (!ft_check_stop(philo->shared))
	{
		ft_print_message(philo->shared, philo->id, "is thinking");
		ft_philo_eat(philo);
		if (ft_check_stop(philo->shared))
			break ;
		ft_print_message(philo->shared, philo->id, "is sleeping");
		ft_precise_sleep(philo->shared->time_to_sleep);
	}
	return (NULL);
}
