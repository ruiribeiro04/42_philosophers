#include "../includes/philo.h"

int main(int ac, char **av)
{
	t_philo	data;
	
	// Validate arguments: check_args returns 1 on success, -1 on error
	if (check_args(ac, av) == -1)
		return (-1);
	init_data(&data, ac, av);
	printf("Num of philos: %i\n", data.n_philo);
	
	return (0);
}


