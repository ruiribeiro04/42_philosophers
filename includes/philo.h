#ifndef PHILO_H
#define PHILO_H

#include <unistd.h>
#include <stdio.h>

	// number_of_philosophers time_to_die time_to_eat
	// time_to_sleep [number_of_times_each_philosopher_must_eat]

typedef struct	s_philo
{
	int n_philo;
	int time_die;
	int time_eat;
	int time_sleep;
	int n_times_eat;
}	t_philo;


// utils.c
int ft_putstr_fd(char *s, int fd);
int	ft_atoi(const char *str);

// data.c
int check_args(int ac, char **av);
void	init_data(t_philo *data, int ac, char **av);

#endif