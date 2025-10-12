#include "philo.h"

void	*monitor_routine(void *arg)
{
	t_shared	*shared;

	shared = (t_shared *)arg;
	while (!check_stop(shared))
	{
		if (check_death(shared))
			break ;
		if (check_all_ate(shared))
			break ;
		usleep(1000);
	}
	return (NULL);
}

int	check_stop(t_shared *shared)
{
	int	stop;

	pthread_mutex_lock(&shared->stop_mutex);
	stop = shared->simulation_stop;
	pthread_mutex_unlock(&shared->stop_mutex);
	return (stop);
}

int	check_death(t_shared *shared)
{
	int			i;
	long long	current_time;
	long long	time_since_meal;

	i = 0;
	while (i < shared->num_philos)
	{
		pthread_mutex_lock(&shared->philos[i].meal_mutex);
		current_time = get_current_time();
		time_since_meal = current_time - shared->philos[i].last_meal_time;
		pthread_mutex_unlock(&shared->philos[i].meal_mutex);
		if (time_since_meal > shared->time_to_die)
		{
			pthread_mutex_lock(&shared->stop_mutex);
			shared->simulation_stop = 1;
			pthread_mutex_unlock(&shared->stop_mutex);
			pthread_mutex_lock(&shared->printf_mutex);
			printf("%lld %d died\n", current_time - shared->start_time,
				shared->philos[i].id);
			pthread_mutex_unlock(&shared->printf_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_all_ate(t_shared *shared)
{
	int	i;
	int	all_done;

	if (!shared->has_must_eat)
		return (0);
	i = 0;
	all_done = 1;
	while (i < shared->num_philos)
	{
		pthread_mutex_lock(&shared->philos[i].meal_mutex);
		if (shared->philos[i].meals_eaten < shared->must_eat_count)
			all_done = 0;
		pthread_mutex_unlock(&shared->philos[i].meal_mutex);
		i++;
	}
	if (all_done)
	{
		pthread_mutex_lock(&shared->stop_mutex);
		shared->simulation_stop = 1;
		pthread_mutex_unlock(&shared->stop_mutex);
		return (1);
	}
	return (0);
}
