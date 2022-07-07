/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 11:35:04 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/07 17:20:01 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static pthread_mutex_t	*init_forks(t_table *table)
{
	pthread_mutex_t	*forks;
	unsigned int	i;

	forks = malloc(sizeof(pthread_mutex_t) * table->nb_philos);
	if (!forks)
		return (error_msg(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
			return (error_msg(STR_ERR_MUTEX, NULL, 0));
		i++;
	}
	return (forks);
}

static void	assign_forks(t_philo *philo)
{
	philo->fork[0] = philo->id;
	philo->fork[1] = (philo->id + 1) % philo->table->nb_philos;
	if (philo->id % 2)
	{
		philo->fork[0] = (philo->id + 1) % philo->table->nb_philos;
		philo->fork[1] = philo->id;
	}
}

static t_philo	**init_philosophers(t_table *table)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * (table->nb_philos + 1));
	if (!philos)
		return (error_msg(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < table->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (error_msg(STR_ERR_MALLOC, NULL, 0));
		if (pthread_mutex_init(&philos[i]->death_lock, 0) != 0)
			return (error_msg(STR_ERR_MUTEX, NULL, 0));
		philos[i]->table = table;
		philos[i]->id = i;
		philos[i]->times_ate = 0;
		assign_forks(philos[i]);
		i++;
	}
	return (philos);
}

static bool	init_global_mutexes(t_table *table)
{
	table->fork_locks = init_forks(table);
	if (!table->fork_locks)
		return (false);
	if (pthread_mutex_init(&table->sim_stop_lock, 0) != 0)
		return (false);
	if (pthread_mutex_init(&table->write_lock, 0) != 0)
		return (false);
	return (true);
}

t_table	*init_table(int ac, char **av, int i)
{
	t_table	*table;

	table = malloc(sizeof(t_table) * 1);
	if (!table)
		return (error_msg(STR_ERR_MALLOC, NULL, 0));
	table->nb_philos = integer_atoi(av[i++]);
	table->time_to_die = integer_atoi(av[i++]);
	table->time_to_eat = integer_atoi(av[i++]);
	table->time_to_sleep = integer_atoi(av[i++]);
	table->must_eat_count = 0;
	if (ac - 1 == 5)
		table->must_eat_count = integer_atoi(av[i]);
	table->philos = init_philosophers(table);
	if (!table->philos)
		return (NULL);
	if (!init_global_mutexes(table))
		return (NULL);
	table->sim_stop = false;
	return (table);
}
