/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 14:07:22 by mcombeau          #+#    #+#             */
/*   Updated: 2022/08/09 16:00:01 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* get_time_in_ms:
*	Gets the current time in miliseconds since the Epoch (1970-01-01 00:00:00).
*	Returns the time value.
*/
time_t	get_time_in_ms(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/* philo_sleep:
*	Pauses the philosopher process for a certain amount of time in miliseconds.
*/
void	philo_sleep(t_philo *philo, time_t sleep_time)
{
	time_t	wake_up;

	if (philo_is_dead(philo))
		return ;
	wake_up = get_time_in_ms() + sleep_time;
	while (get_time_in_ms() < wake_up)
	{
		if (philo_is_dead(philo))
			break ;
		usleep(100);
	}
}

/* sim_start_delay:
*	Waits for a small delay at the beginning of each process execution
*	so that all processes start at the same time with the same start time
*	reference. This ensures all processes and the grim reaper threads are
*	synchronized.
*/
void	sim_start_delay(time_t start_time)
{
	while (get_time_in_ms() < start_time)
		continue ;
}
