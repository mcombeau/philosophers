/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 11:46:06 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/03 12:36:30 by mcombeau         ###   ########.fr       */
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

void	*free_table(t_table *table)
{
	int	i;

	if (!table)
		return (NULL);
	if (table->fork_locks != NULL)
		free(table->fork_locks);
	if (table->philos != NULL)
	{
		i = 0;
		while (i < table->nb_philos)
		{
			if (table->philos[i] != NULL)
				free(table->philos[i]);
			i++;
		}
		free(table->philos);
	}
	free(table);
	return (NULL);
}

void	destroy_mutexes(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		pthread_mutex_destroy(&table->fork_locks[i]);
		pthread_mutex_destroy(&table->philos[i]->eat_lock);
		i++;
	}
	pthread_mutex_destroy(&table->write_lock);
}

int	stop_simulation(t_table	*table)
{
	int	i;
	int	full_count;

	if (table->must_eat_count != -1)
	{
		write_outcome(table);
	}
	destroy_mutexes(table);
	free_table(table);
	return (1);
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
	stop_simulation(table);
//	free_table(table);
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
	