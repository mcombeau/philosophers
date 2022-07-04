/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 11:35:04 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/04 13:21:02 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t	*init_forks(t_table *table)
{
	pthread_mutex_t	*forks;
	unsigned int	i;

	forks = malloc(sizeof(pthread_mutex_t) * table->nb_philos);
	if (!forks)
	{
		msg(STR_ERR_MALLOC, NULL, 0);
		return (NULL);
	}
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
		{
			msg(STR_ERR_MUTEX, NULL, 0);
			return (NULL);
		}
		i++;
	}
	return (forks);
}

t_philo	**init_philosophers(t_table *table)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * (table->nb_philos + 1));
	if (!philos)
	{
		msg(STR_ERR_MALLOC, NULL, 0);
		return (NULL);
	}
	i = 0;
	while (i < table->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
		{
			msg(STR_ERR_MALLOC, NULL, 0);
			return (NULL);
		}
		if (pthread_mutex_init(&philos[i]->eat_lock, 0) != 0)
		{
			msg(STR_ERR_MUTEX, NULL, 0);
			return (NULL);
		}
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
	{
		msg(STR_ERR_MALLOC, NULL, 0);
		return (NULL);
	}
	table->nb_philos = integer_atoi(av[i++]);
	if (table->nb_philos > MAX_PHILOS)
	{
		msg(STR_ERR_INPUT_PHILO_OFLOW, STR_MAX_PHILOS, 0);
		return (free_table(table));
	}
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
