/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 15:12:00 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/13 16:53:24 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* eat_routine:
*	When a philosopher is ready to eat, he will wait for his fork mutexes to
*	be unlocked before locking them. Then the philosopher will eat for a certain
*	amount of time. The time of the last meal is recorded at the beginning of
*	the meal, not at the end, as per the subject's requirements.
*/
static void	eat_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	write_status(philo, false, GOT_FORK_1);
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[1]]);
	write_status(philo, false, GOT_FORK_2);
	write_status(philo, false, EATING);
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->meal_time_lock);
	philo_sleep(philo->table, philo->table->time_to_eat);
	if (has_simulation_stopped(philo->table) == false)
	{
		pthread_mutex_lock(&philo->meal_time_lock);
		philo->times_ate += 1;
		pthread_mutex_unlock(&philo->meal_time_lock);
	}
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[1]]);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
}

/* sleep_routine:
*	The philosopher sleeps for time_to_sleep. Since the philosopher was
*	eating right before sleeping, he unlocks his forks after writing his
*	sleeping status. This is so that the next philosopher cannot pick up
*	one of these forks before they are explicitely unlocked with this
*	philosopher's sleep message.
*/
static void	sleep_routine(t_philo *philo)
{
	write_status(philo, false, SLEEPING);
	philo_sleep(philo->table, philo->table->time_to_sleep);
}

/* think_routine:
*	Once a philosopher is done sleeping, he will think for a certain
*	amount of time before starting to eat again.
*	The time_to_think is calculated depending on how long it has been
*	since the philosopher's last meal, the time_to_eat and the time_to_die
*	to determine when the philosopher will be hungry again.
*	This helps stagger philosopher's eating routines to avoid forks being
*	needlessly monopolized by one philosopher to the detriment of others.
*/
static void	think_routine(t_philo *philo, bool silent)
{
	time_t	time_to_think;

	pthread_mutex_lock(&philo->meal_time_lock);
	time_to_think = (philo->table->time_to_die
			- (get_time_in_ms() - philo->last_meal)
			- philo->table->time_to_eat) / 2;
	pthread_mutex_unlock(&philo->meal_time_lock);
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

/* lone_philo_routine:
*	This routine is invoked when there is only a single philosopher.
*	A single philosopher only has one fork, and so cannot eat. The
*	philosopher will pick up that fork, wait as long as time_to_die and die.
*	This is a separate routine to make sure that the thread does not get
*	stuck waiting for the second fork in the eat routine.
*/
static void	*lone_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_locks[philo->fork[0]]);
	write_status(philo, false, GOT_FORK_1);
	philo_sleep(philo->table, philo->table->time_to_die);
	write_status(philo, false, DIED);
	pthread_mutex_unlock(&philo->table->fork_locks[philo->fork[0]]);
	return (NULL);
}

/* philosopher:
*	The philosopher thread routine. The philosopher must eat, sleep
*	and think. In order to avoid conflicts between philosopher threads,
*	philosophers with an even id start by thinking, which delays their
*	meal time by a small margin. This allows odd-id philosophers to
*	grab their forks first, avoiding deadlocks.
*/
void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->meal_time_lock);
	philo->last_meal = philo->table->start_time;
	pthread_mutex_unlock(&philo->meal_time_lock);
	sim_start_delay(philo->table->start_time);
	if (philo->table->time_to_die == 0)
		return (NULL);
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
