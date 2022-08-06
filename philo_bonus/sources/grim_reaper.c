/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grim_reaper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 12:00:18 by mcombeau          #+#    #+#             */
/*   Updated: 2022/08/06 12:53:09 by mcombeau         ###   ########.fr       */
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
*	those decrementations to count how many philosophers are full.
*/
void	*global_grim_reaper(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	sim_start_delay(table->start_time + 100);
	while (table->philo_full_count < table->nb_philos)
	{
		sem_wait(table->sem_philo_full);
		table->philo_full_count += 1;
	}
	sem_wait(table->sem_all_ate_enough);
	table->all_philos_full = true;
	kill_all_philos(table, EXIT_SUCCESS);
	sem_post(table->sem_all_ate_enough);
	return (NULL);
}

/* end_condition_reached:
*	Checks this philosopher to see if one of two end conditions
*	has been reached. Exits the process with specific exit codes
*	if the philosopher has died or has eaten enough.
*	Returns false if the philosopher is alive and well.
*/
static bool	end_condition_reached(t_table *table, t_philo *philo)
{
	sem_wait(philo->sem_meal);
	if (get_time_in_ms() - philo->last_meal >= table->time_to_die)
	{
		write_status(philo, true, DIED);
		child_exit(table, CHILD_EXIT_PHILO_DEAD);
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
*	be killed and if he ate enough. If one of those two
*	end conditions are reached, terminate the philosopher process
*	with the appropriate exit code.
*/
void	*personal_grim_reaper(void *data)
{
	t_table			*table;

	table = (t_table *)data;
	sim_start_delay(table->start_time);
	if (table->must_eat_count == 0)
		child_exit(table, CHILD_EXIT_PHILO_FULL);
	while (!end_condition_reached(table, table->this_philo))
		usleep(5000);
	return (NULL);
}
