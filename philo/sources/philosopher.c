/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 15:12:00 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/06 14:56:07 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eat_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	write_status(philo->table, philo->id, GOT_FORK);
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[1]]);
	write_status(philo->table, philo->id, GOT_FORK);
	write_status(philo->table, philo->id, EATING);
	pthread_mutex_lock(&philo->death_lock);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->death_lock);
	philo_sleep(philo->table, philo->table->time_to_eat);
	if (has_simulation_stopped(philo->table) == false)
		philo->times_ate += 1;
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[1]]);
}

static void	sleep_routine(t_philo *philo)
{
	write_status(philo->table, philo->id, SLEEPING);
	philo_sleep(philo->table, philo->table->time_to_sleep);
}

static void	think_routine(t_philo *philo)
{
	write_status(philo->table, philo->id, THINKING);
}

static void	*lone_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	write_status(philo->table, philo->id, GOT_FORK);
	philo_sleep(philo->table, philo->table->time_to_die);
	write_status(philo->table, philo->id, DIED);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
	return (NULL);
}

void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	sim_start_delay(philo->table->start_time);
	philo->last_meal = get_time_in_ms();
	if (philo->table->nb_philos == 1)
		return (lone_philo_routine(philo));
	else if (!(philo->id % 2))
	{
		if (philo->table->time_to_sleep != 0)
		{
			sleep_routine(philo);
			think_routine(philo);
		}
		else
			philo_sleep(philo->table, 10);
	}
	while (has_simulation_stopped(philo->table) == false)
	{
		eat_routine(philo);
		sleep_routine(philo);
		think_routine(philo);
	}
	return (NULL);
}
