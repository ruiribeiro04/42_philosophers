/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruiferna <ruiferna@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 10:23:57 by ruiferna          #+#    #+#             */
/*   Updated: 2025/10/15 12:02:03 by ruiferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_set_basic_args(char **argv, t_shared *shared)
{
	if (!ft_is_valid_number(argv[1]) || !ft_is_valid_number(argv[2])
		|| !ft_is_valid_number(argv[3]) || !ft_is_valid_number(argv[4]))
		return (0);
	shared->num_philos = ft_atoi(argv[1]);
	shared->time_to_die = ft_atoi(argv[2]);
	shared->time_to_eat = ft_atoi(argv[3]);
	shared->time_to_sleep = ft_atoi(argv[4]);
	if (shared->num_philos <= 0 || shared->time_to_die <= 0
		|| shared->time_to_eat <= 0 || shared->time_to_sleep <= 0)
		return (0);
	return (1);
}

int	ft_parse_optional_arg(int argc, char **argv, t_shared *shared)
{
	if (argc == 6)
	{
		if (!ft_is_valid_number(argv[5]))
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

int	ft_parse_arguments(int argc, char **argv, t_shared *shared)
{
	if (argc != 5 && argc != 6)
	{
		printf("Usage: %s number_of_philosophers", argv[0]);
		printf(" time_to_die time_to_eat ");
		printf("time_to_sleep [number_of_times_must_eat]\n");
		return (0);
	}
	if (!ft_set_basic_args(argv, shared))
		return (0);
	if (!ft_parse_optional_arg(argc, argv, shared))
		return (0);
	return (1);
}
