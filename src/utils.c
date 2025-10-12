/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruiferna <ruiferna@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 02:10:52 by ruiferna          #+#    #+#             */
/*   Updated: 2025/10/12 02:11:52 by ruiferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_message(t_shared *shared, int philo_id, char *message)
{
	long long	timestamp;

	pthread_mutex_lock(&shared->stop_mutex);
	if (!shared->simulation_stop)
	{
		pthread_mutex_unlock(&shared->stop_mutex);
		pthread_mutex_lock(&shared->printf_mutex);
		timestamp = get_current_time() - shared->start_time;
		printf("%lld %d %s\n", timestamp, philo_id, message);
		pthread_mutex_unlock(&shared->printf_mutex);
	}
	else
		pthread_mutex_unlock(&shared->stop_mutex);
}

int	is_valid_number(const char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

long long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL));
}

void	precise_sleep(int ms)
{
	long long	start_time;

	start_time = get_current_time();
	while ((get_current_time() - start_time) < ms)
		usleep(100);
}

int	ft_atoi(const char *str)
{
	int	result;
	int	i;

	result = 0;
	i = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result);
}
