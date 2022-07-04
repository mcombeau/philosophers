/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 11:46:06 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/04 11:23:48 by mcombeau         ###   ########.fr       */
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
			return (exit_error(STR_ERR_THREAD, table));
		i++;
		usleep(100);
	}
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_create(&table->philos[i]->grim_reaper, NULL,
				&grim_reaper, table->philos[i]) != 0)
			return (exit_error(STR_ERR_THREAD, table));
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
	pthread_mutex_unlock(&table->write_lock);
	pthread_mutex_destroy(&table->write_lock);
}

int	stop_simulation(t_table	*table)
{
	int	i;

//	pthread_mutex_unlock(&table->write_lock);
	i = 0;
	while (i < table->nb_philos)
	{
		pthread_detach(table->philos[i]->thread);
		pthread_detach(table->philos[i]->grim_reaper);
		i++;
	}
	if (table->must_eat_count != -1)
		write_outcome(table);
	destroy_mutexes(table);
	free_table(table);
	return (1);
}

int	main(int ac, char **av)
{
	t_table	*table;

	table = NULL;
	if (ac - 1 < 4 || ac - 1 > 5)
		return (msg(STR_USAGE, NULL, EXIT_FAILURE));
	if (is_valid_input(ac, av) == 0)
		return (EXIT_FAILURE);
	table = init_table(ac, av, 1);
	if (!table)
		return (exit_error("Could not initialize table.\n", NULL));
	start_simulation(table);
	stop_simulation(table);
	return (EXIT_SUCCESS);
}
