/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 15:12:00 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/07 16:58:57 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eat_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	write_status(philo, false, GOT_FORK_1);
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[1]]);
	write_status(philo, false, GOT_FORK_2);
	write_status(philo, false, EATING);
	pthread_mutex_lock(&philo->death_lock);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->death_lock);
	philo_sleep(philo->table, philo->table->time_to_eat);
	if (has_simulation_stopped(philo->table) == false)
		philo->times_ate += 1;
}

static void	sleep_routine(t_philo *philo)
{
	write_status(philo, false, SLEEPING);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[1]]);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
	philo_sleep(philo->table, philo->table->time_to_sleep);
}

static void	think_routine(t_philo *philo, bool silent)
{
	time_t	time_to_think;

	time_to_think = (philo->table->time_to_die
			- (get_time_in_ms() - philo->last_meal)
			- philo->table->time_to_eat) / 2;
	if (time_to_think < 0)
		time_to_think = 0;
	if (time_to_think == 0 && silent == true)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (silent == false)
		write_status(philo, false, THINKING);
	philo_sleep(philo->table, time_to_think);
}

static void	*lone_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	write_status(philo, false, GOT_FORK_1);
	philo_sleep(philo->table, philo->table->time_to_die);
	write_status(philo, false, DIED);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
	return (NULL);
}

void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	sim_start_delay(philo->table->start_time);
	if (philo->table->time_to_die == 0)
		return (NULL);
	philo->last_meal = get_time_in_ms();
	if (philo->table->nb_philos == 1)
		return (lone_philo_routine(philo));
	else if (philo->id % 2)
		think_routine(philo, true);
	while (has_simulation_stopped(philo->table) == false)
	{
		eat_routine(philo);
		sleep_routine(philo);
		think_routine(philo, false);
	}
	return (NULL);
}
