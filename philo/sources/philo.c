/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 11:46:06 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/04 17:21:57 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	start_simulation(t_table *table)
{
	unsigned int	i;

	table->start_time = get_time_in_ms() + table->nb_philos;
	i = 0;
	while (i < table->nb_philos)
	{
		table->philos[i]->last_meal = get_time_in_ms();
		if (pthread_create(&table->philos[i]->thread, NULL,
				&philosopher, table->philos[i]) != 0)
			return (exit_error(STR_ERR_THREAD, NULL, table));
		i++;
	}
	if (table->nb_philos > 1)
	{
		if (pthread_create(&table->grim_reaper, NULL,
				&grim_reaper, table) != 0)
			return (exit_error(STR_ERR_THREAD, NULL, table));
	}
	return (true);
}

void	*free_table(t_table *table)
{
	unsigned int	i;

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
	unsigned int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		pthread_mutex_destroy(&table->fork_locks[i]);
		pthread_mutex_destroy(&table->philos[i]->death_lock);
		i++;
	}
	pthread_mutex_destroy(&table->write_lock);
	pthread_mutex_destroy(&table->sim_stop_lock);
}

int	stop_simulation(t_table	*table)
{
	unsigned int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		printf("Joining threads.\n");
		pthread_join(table->philos[i]->thread, NULL);
		printf("Philo #%d thread joined.\n", table->philos[i]->id + 1);
		i++;
	}
	if (table->nb_philos > 1)
		pthread_join(table->grim_reaper, NULL);
	printf("Grim reaper thread joined.\n");
	if (table->must_eat_count != 0)
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
	if (!is_valid_input(ac, av))
		return (EXIT_FAILURE);
	table = init_table(ac, av, 1);
	if (!table)
		return (EXIT_FAILURE);
	if (!start_simulation(table))
		return (EXIT_FAILURE);
	stop_simulation(table);
	return (EXIT_SUCCESS);
}
