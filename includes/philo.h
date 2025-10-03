#ifndef PHILO_H
#define PHILO_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

	// number_of_philosophers time_to_die time_to_eat
	// time_to_sleep [number_of_times_each_philosopher_must_eat]

typedef struct	s_data
{
	int n_philo;
	int time_die;
	int time_eat;
	int time_sleep;
	int n_times_eat;
}	t_data;

typedef struct	s_philo
{
	int id;
	int	alive;
	int	left_fork;
	int	right_fork;
	pthread_t	thread;
	struct s_data *data;
}	t_philo;


// utils.c
int ft_putstr_fd(char *s, int fd);
int	ft_atoi(const char *str);

// data.c
int check_args(int ac, char **av);
void	init_data(t_data *data, int ac, char **av);

// philo threads
void *philo_routine(void *arg);

#endif