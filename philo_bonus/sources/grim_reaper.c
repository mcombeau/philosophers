/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grim_reaper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 12:00:18 by mcombeau          #+#    #+#             */
/*   Updated: 2022/08/05 11:55:56 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* kill_all_philos:
*	TODO
*/
int	kill_all_philos(t_table *table, int exit_code)
{
	unsigned int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		kill(table->pids[i], SIGTERM);
		i++;
	}
	return (exit_code);
}

/* end_condition_reached:
*	Checks each philosopher to see if one of two end conditions
*	has been reached. Stops the simulation if a philosopher needs
*	to be killed, or if every philosopher has eaten enough.
*	Returns true if an end condition has been reached, false if not.
*/
static bool	end_condition_reached(t_table *table, t_philo *philo)
{
	sem_wait(philo->sem_meal);
	if (get_time_in_ms() - philo->last_meal >= table->time_to_die)
	{
		write_status(philo, true, DIED);
		child_exit(table, CHILD_EXIT_PHILO_DEAD);
	}
	if (table->must_eat_count != -1 && philo->times_ate >= (unsigned int)table->must_eat_count)
	{
		while (philo->nb_forks_held != 0)
		{
			sem_post(philo->sem_forks);
			philo->nb_forks_held -= 1;
		}
		child_exit(table, CHILD_EXIT_PHILO_FULL);
	}
	sem_post(philo->sem_meal);
	return (false);
}

/* personal_grim_reaper:
*	The grim reaper thread's routine. Checks if a philosopher must
*	be killed and if all philosophers ate enough. If one of those two
*	end conditions are reached, it stops the simulation.
*/
void	*personal_grim_reaper(void *data)
{
	t_table			*table;

	table = (t_table *)data;
	if (table->must_eat_count == 0)
		child_exit(table, CHILD_EXIT_PHILO_FULL);
	sim_start_delay(table->start_time);
	while (!end_condition_reached(table, table->this_philo))
		usleep(5000);
	return (NULL);
}
