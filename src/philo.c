#include "../includes/philo.h"

void *philo_routine(void *arg)
{
	t_philo *ph;

	ph = (t_philo *)arg;
	printf("Philosopher %d starting\n", ph->id);
	sleep(1);
	printf("Philosopher %d is eating\n", ph->id);
	sleep(1);
	printf("Philosopher %d is thinking\n", ph->id);
	sleep(1);
	printf("Philosopher %d finished\n", ph->id);
	return (NULL);
}

int main(int ac, char **av)
{
	t_data	data;
	t_philo *philos;
	int i;

	if (check_args(ac, av) == -1)
		return (-1);
	init_data(&data, ac, av);
	printf("Num of philos: %i\n", data.n_philo);

	philos = malloc(sizeof(t_philo) * data.n_philo);
	if (!philos)
	{
		perror("malloc");
		return (1);
	}
	i = 0;
	while (i < data.n_philo)
	{
		philos[i].id = i + 1;
		philos[i].alive = 1;
		philos[i].left_fork = -1;
		philos[i].right_fork = -1;
		philos[i].data = &data;
		if (pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]) != 0)
		{
			perror("pthread_create");
			free(philos);
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < data.n_philo)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}

	free(philos);
	return (0);
}


