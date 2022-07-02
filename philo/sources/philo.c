/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 11:46:06 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/02 16:52:37 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_simulation(t_table *table)
{
	int	i;

	table->start_time = get_time_in_ms();
	i = 0;
	while (i < table->nb_philos)
	{
		table->philos[i]->last_meal = get_time_in_ms();
		if (pthread_create(&table->philos[i]->thread, NULL,
				&philosopher, table->philos[i]) != 0)
			return (exit_error("Could not create thread.\n", table));
		i++;
		usleep(100);
	}
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_create(&table->philos[i]->grim_reaper, NULL,
				&grim_reaper, table->philos[i]) != 0)
			return (exit_error("Could not create thread.\n", table));
		i++;
		usleep(100);
	}
	while (table->all_alive)
		continue ;
	return (1);
}

pthread_mutex_t	*init_forks(t_table *table)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t) * table->nb_philos);
	if (!forks)
		return (NULL);
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
			return (NULL);
		i++;
	}
	return (forks);
}

t_philo	**init_philosophers(t_table *table)
{
	t_philo	**philos;
	int		i;

	philos = malloc(sizeof(t_philo) * (table->nb_philos + 1));
	if (!philos)
		return (NULL);
	i = 0;
	while (i < table->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (NULL);
		if (pthread_mutex_init(&philos[i]->eat_lock, 0) != 0)
			return (NULL);
		philos[i]->table = table;
		philos[i]->id = i;
		philos[i]->is_eating = 0;
		philos[i]->times_ate = 0;
		philos[i]->left_fork = i;
		philos[i]->right_fork = (i + 1) % table->nb_philos;
		i++;
	}
	return (philos);
}

t_table	*init_table(int ac, char **av, int i)
{
	t_table	*table;

	table = malloc(sizeof(t_table) * 1);
	if (!table)
		return (NULL);
	table->nb_philos = integer_atoi(av[i++]);
	table->time_to_die = integer_atoi(av[i++]);
	table->time_to_eat = integer_atoi(av[i++]);
	table->time_to_sleep = integer_atoi(av[i++]);
	table->must_eat_count = -1;
	if (ac - 1 == 5)
		table->must_eat_count = integer_atoi(av[i]);
	table->all_alive = 1;
	table->fork_locks = init_forks(table);
	if (!table->fork_locks)
		return (free_table(table));
	table->philos = init_philosophers(table);
	if (!table->philos)
		return (free_table(table));
	if (pthread_mutex_init(&table->write_lock, 0) != 0)
		return (free_table(table));
	return (table);
}

int	main(int ac, char **av)
{
	t_table	*table;

	table = NULL;
	if (ac - 1 < 4 || ac - 1 > 5)
		return (exit_usage());
	if (is_valid_input(ac, av) == 0)
		return (exit_error("Invalid input.\n", NULL));
	table = init_table(ac, av, 1);
	if (!table)
		return (exit_error("Could not initialize table.\n", NULL));
	start_simulation(table);
	free_table(table);
	return (EXIT_SUCCESS);
}
/*
	printf("Nb philos = %d\nTime to die = %d\n\
	Time to eat = %d\nTime to sleep = %d\n\
	Must eat count = %d\n", 
	table->nb_philos, table->time_to_die, 
	table->time_to_eat, table->time_to_sleep, 
	table->must_eat_count);
	get_time_in_ms();
	usleep(2);
	get_time_in_ms();
	usleep(500);
	get_time_in_ms();
	usleep(1000);
	get_time_in_ms();
	free(table);*/
	