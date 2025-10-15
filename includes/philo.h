/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ruiferna <ruiferna@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 02:11:31 by ruiferna          #+#    #+#             */
/*   Updated: 2025/10/15 14:15:13 by ruiferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_mutex_t	meal_mutex;
	struct s_shared	*shared;
}					t_philo;

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
}					t_shared;

// args.c
int			ft_set_basic_args(char **argv, t_shared *shared);
int			ft_parse_optional_arg(int argc,
				char **argv, t_shared *shared);
int			ft_parse_arguments(int argc, char **argv, t_shared *shared);

// init.c
int			ft_init_shared(t_shared *shared);
void		ft_init_philosophers(t_shared *shared);

// monitor.c
void		*ft_monitor_routine(void *arg);
int			ft_check_stop(t_shared *shared);
int			ft_check_death(t_shared *shared);
int			ft_check_all_ate(t_shared *shared);

// philo.c
void		ft_philo_eat(t_philo *philo);
void		*ft_philo_routine(void *arg);

// threads.c
int			ft_allocate_threads(t_shared *shared, pthread_t **threads);
void		ft_start_philosophers(t_shared *shared, pthread_t *threads,
				pthread_t *monitor);
void		ft_join_philosophers(t_shared *shared, pthread_t *threads,
				pthread_t monitor);

// utils.c
void		ft_print_message(t_shared *shared, int philo_id,
				char *message);
int			ft_is_valid_number(const char *str);
int			ft_atoi(const char *str);
long long	ft_get_current_time(void);
void		ft_precise_sleep(int ms);

// main.c
int			main(int argc, char **argv);
void		ft_cleanup(t_shared *shared, pthread_t *threads);

#endif