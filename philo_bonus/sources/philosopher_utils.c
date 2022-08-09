/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 13:53:19 by mcombeau          #+#    #+#             */
/*   Updated: 2022/08/09 16:37:07 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* grab_fork:
*	Wait for the fork semaphore to allow the philosopher to take a
*	fork. When the philosopher takes a fork, the semaphore is decreased
*	by 1 to represent the fact that there is one less fork available
*	in the pool of forks.
*	There is no real distinction between the first and second fork a
*	philosopher takes, the dictinction is made here only for debut purposes.
*/
void	grab_fork(t_philo *philo)
{
	if (philo_is_dead(philo))
		return ;
	sem_wait(philo->sem_forks);
	sem_wait(philo->sem_meal);
	if (philo->nb_forks_held <= 0)
		write_status(philo, false, GOT_FORK_1);
	if (philo->nb_forks_held == 1)
		write_status(philo, false, GOT_FORK_2);
	philo->nb_forks_held += 1;
	sem_post(philo->sem_meal);
}

/* philo_is_dead:
*	Checks whether the calling philosopher is dead.
*	Returns true if the philosopher is dead, false if not.
*/
bool	philo_is_dead(t_philo *philo)
{
	bool	r;

	r = true;
	sem_wait(philo->sem_dead);
	if (philo->is_dead == false)
		r = false;
	sem_post(philo->sem_dead);
	return (r);
}
