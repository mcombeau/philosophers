/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grim_reaper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 12:00:18 by mcombeau          #+#    #+#             */
/*   Updated: 2022/08/09 12:55:29 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* kill_all_philos:
*	Sends the kill signal to all philosopher processes. Used to send the
*	simulation if a philosopher has died or if a philosopher process has
*	encountered a fatal error.
*	Returns the given exit code.
*/
int	kill_all_philos(t_table *table, int exit_code)
{
	unsigned int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		kill(table->pids[i], SIGKILL);
		i++;
	}
	return (exit_code);
}

/* global_grim_reaper:
*	Kills all philosophers if each one has eaten enough. Each philosopher
*	process decrements the sem_philo_full semaphore. This thread registers
*	those decrementations to count how many philosophers have eaten enough.
*/
void	*global_grim_reaper(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	if (table->must_eat_count < 0 || table->time_to_die == 0)
		return (NULL);
	sim_start_delay(table->start_time);
	while (table->philo_full_count < table->nb_philos)
	{
		if (has_simulation_stopped(table) == true)
			return (NULL);
		sem_wait(table->sem_philo_full);
		if (has_simulation_stopped(table) == false)
			table->philo_full_count += 1;
	}
	sem_wait(table->sem_stop);
	table->stop_sim = true;
	kill_all_philos(table, EXIT_SUCCESS);
	sem_post(table->sem_stop);
	return (NULL);
}

/* end_condition_reached:
*	Checks this philosopher to see if one of two end conditions
*	has been reached. Exits the process with a specific exit code
*	if the philosopher has died. If the philosopher has eaten enough,
*	marks it by decrementing a semaphore.
*	Returns false if the philosopher is alive.
*/
static bool	end_condition_reached(t_table *table, t_philo *philo)
{
	sem_wait(philo->sem_meal);
	if (get_time_in_ms() - philo->last_meal >= table->time_to_die)
	{
		write_status(philo, true, DIED);
		return (true);
	}
	if (table->must_eat_count != -1 && philo->ate_enough == false
		&& philo->times_ate >= (unsigned int)table->must_eat_count)
	{
		sem_post(philo->sem_philo_full);
		philo->ate_enough = true;
	}
	sem_post(philo->sem_meal);
	return (false);
}

/* personal_grim_reaper:
*	The grim reaper thread's routine. Checks if this philosopher must
*	be killed and if he ate enough. If the philosopher is dead,
*	terminate the philosopher process with the appropriate exit code.
*/
void	*personal_grim_reaper(void *data)
{
	t_table			*table;

	table = (t_table *)data;
	sim_start_delay(table->start_time);
	if (table->must_eat_count == 0)
		return (NULL);
	while (!end_condition_reached(table, table->this_philo))
		continue ;
	child_exit(table, CHILD_EXIT_PHILO_DEAD);
	return (NULL);
}
