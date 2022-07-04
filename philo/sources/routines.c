/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 15:12:00 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/04 13:20:29 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eat_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->left_fork]);
	write_status(philo->table, philo->id, STR_FORK, PURPLE);
	pthread_mutex_lock(&philo->table->fork_locks[philo->right_fork]);
	write_status(philo->table, philo->id, STR_FORK, PURPLE);
	pthread_mutex_lock(&philo->eat_lock);
	write_status(philo->table, philo->id, STR_EAT, GREEN);
	philo->is_eating = 1;
	philo->last_meal = get_time_in_ms();
	usleep(philo->table->time_to_eat * 1000 - 16000);
	while (get_time_in_ms() - philo->last_meal < philo->table->time_to_eat)
		continue ;
	philo->times_ate += 1;
	philo->is_eating = 0;
	pthread_mutex_unlock(&philo->eat_lock);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->left_fork]);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->right_fork]);
}

static void	sleep_routine(t_philo *philo)
{
	time_t	time;

	write_status(philo->table, philo->id, STR_SLEEP, CYAN);
	time = get_time_in_ms();
	usleep(philo->table->time_to_sleep * 1000 - 16000);
	while (get_time_in_ms() - time < philo->table->time_to_sleep)
		continue ;
}

static void	think_routine(t_philo *philo)
{
	write_status(philo->table, philo->id, STR_THINK, CYAN);
}

void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	while (philo->table->all_alive)
	{
		eat_routine(philo);
		sleep_routine(philo);
		think_routine(philo);
		usleep(100);
	}
	return (NULL);
}
