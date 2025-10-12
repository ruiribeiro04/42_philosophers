#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_mutex_t	meal_mutex;
	struct s_shared	*shared;
}	t_philo;

typedef struct s_shared
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	int				has_must_eat;
	int				simulation_stop;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	printf_mutex;
	pthread_mutex_t	stop_mutex;
	t_philo			*philos;
}	t_shared;


// init.c
int	init_shared(t_shared *shared);
void	init_philosophers(t_shared *shared);

// monitor.c
void	*monitor_routine(void *arg);
int	check_stop(t_shared *shared);
int	check_death(t_shared *shared);
int	check_all_ate(t_shared *shared);

// philo.c
void	ft_philo_eat(t_philo *philo);
void	*philo_routine(void *arg);

// time.c
long long	get_current_time(void);
void	precise_sleep(int ms);

// utils.c
void	print_message(t_shared *shared, int philo_id, char *message);
int	is_valid_number(const char *str);
int	ft_atoi(const char *str);

#endif