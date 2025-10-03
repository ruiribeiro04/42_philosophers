#include "philo.h"

static int	is_numeric(const char *s)
{
	int i;

	if (!s || s[0] == '\0')
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int check_args(int ac, char **av)
{
	// number_of_philosophers time_to_die time_to_eat
	// time_to_sleep [number_of_times_each_philosopher_must_eat]
	if (ac < 5 || ac > 6)
	{
		ft_putstr_fd("Usage: ./philo number_of_philosophers ", 1);
		ft_putstr_fd("time_to_die time_to_eat time_to_sleep ", 1);
		return (ft_putstr_fd("[number_of_times_each_philosopher_must_eat]\n", 1), -1);
	}
	/* Require arguments to be strictly numeric (no trailing letters like "10a") */
	if (!is_numeric(av[1]) || !is_numeric(av[2]) || !is_numeric(av[3])
			|| !is_numeric(av[4]) || (ac == 6 && !is_numeric(av[5])))
		return (ft_putstr_fd("Error: All arguments must be positive integers\n", 2), -1);
	if (ft_atoi(av[1]) <= 0 || ft_atoi(av[1]) > 200)
		return (ft_putstr_fd("Error: Numbers of philos is invalid!\n", 2), -1);
	if (ft_atoi(av[2]) < 0 || ft_atoi(av[3]) < 0 || ft_atoi(av[4]) < 0)
		return (ft_putstr_fd("Error: one of time values invalid!\n", 2), -1);
	if (ac == 6)
		if (ft_atoi(av[5]) < 0)
			return (ft_putstr_fd("Error: Num times to eat is invalid!\n", 2), -1);
	return (1);
}

void	init_data(t_data *data, int ac, char **av)
{
	data->n_philo = ft_atoi(av[1]);
	data->time_die = ft_atoi(av[2]);
	data->time_eat = ft_atoi(av[3]);
	data->time_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->n_times_eat = ft_atoi(av[5]);
	else
		data->n_times_eat = 0;
}
