/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 15:12:00 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/05 16:33:47 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eat_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->left_fork]);
	write_status(philo->table, philo->id, STR_FORK, PURPLE);
	pthread_mutex_lock(&philo->table->fork_locks[philo->right_fork]);
	write_status(philo->table, philo->id, STR_FORK, PURPLE);
	write_status(philo->table, philo->id, STR_EAT, GREEN);
	pthread_mutex_lock(&philo->death_lock);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->death_lock);
	philo_sleep(philo->table, philo->table->time_to_eat);
	if (has_simulation_stopped(philo->table) == false)
		philo->times_ate += 1;
	pthread_mutex_unlock(&philo->table->fork_locks[philo->left_fork]);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->right_fork]);
}

static void	sleep_routine(t_philo *philo)
{
	write_status(philo->table, philo->id, STR_SLEEP, CYAN);
	philo_sleep(philo->table, philo->table->time_to_sleep);
}

static void	think_routine(t_philo *philo)
{
	write_status(philo->table, philo->id, STR_THINK, CYAN);
}

static void	*lone_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->left_fork]);
	write_status(philo->table, philo->id, STR_FORK, PURPLE);
	philo_sleep(philo->table, philo->table->time_to_die);
	write_status(philo->table, philo->id, STR_DIE, RED);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->left_fork]);
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
		sleep_routine(philo);
		think_routine(philo);
	}
	while (has_simulation_stopped(philo->table) == false)
	{
		eat_routine(philo);
		sleep_routine(philo);
		think_routine(philo);
	}
	return (NULL);
}
