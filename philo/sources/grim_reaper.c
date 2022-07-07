/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grim_reaper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 12:00:18 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/07 17:00:51 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	set_sim_stop_flag(t_table *table, bool state)
{
	pthread_mutex_lock(&table->sim_stop_lock);
		table->sim_stop = state;
	pthread_mutex_unlock(&table->sim_stop_lock);
}

bool	has_simulation_stopped(t_table *table)
{
	bool	r;

	r = false;
	pthread_mutex_lock(&table->sim_stop_lock);
	if (table->sim_stop == true)
		r = true;
	pthread_mutex_unlock(&table->sim_stop_lock);
	return (r);
}

static bool	kill_philo(t_philo *philo)
{
	time_t	time;

	time = get_time_in_ms();
	if ((time - philo->last_meal) >= philo->table->time_to_die)
	{
		set_sim_stop_flag(philo->table, true);
		write_status(philo, true, DIED);
		pthread_mutex_unlock(&philo->death_lock);
		return (true);
	}
	return (false);
}

static bool	end_condition_reached(t_table *table)
{
	unsigned int	i;
	bool			all_ate_enough;

	i = 0;
	all_ate_enough = true;
	while (i < table->nb_philos)
	{
		pthread_mutex_lock(&table->philos[i]->death_lock);
		if (kill_philo(table->philos[i]))
			return (true);
		if (table->philos[i]->times_ate < table->must_eat_count)
			all_ate_enough = false;
		pthread_mutex_unlock(&table->philos[i]->death_lock);
		i++;
	}
	if (table->must_eat_count != 0 && all_ate_enough == true)
	{
		set_sim_stop_flag(table, true);
		return (true);
	}
	return (false);
}

void	*grim_reaper(void *data)
{
	t_table			*table;

	table = (t_table *)data;
	set_sim_stop_flag(table, false);
	sim_start_delay(table->start_time);
	while (true)
	{
		if (end_condition_reached(table) == true)
			return (NULL);
	}
	return (NULL);
}
