/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grim_reaper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 12:00:18 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/05 16:09:08 by mcombeau         ###   ########.fr       */
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

static bool	philo_is_dying(t_philo *philo)
{
	time_t	time;

	time = get_time_in_ms();
	if (time - philo->last_meal >= philo->table->time_to_die)
	{
		set_sim_stop_flag(philo->table, true);
		pthread_mutex_lock(&philo->table->write_lock);
		printf(STR_STATUS, time - philo->table->start_time,
			RED, philo->id + 1, STR_DIE);
		pthread_mutex_unlock(&philo->table->write_lock);
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
		if (philo_is_dying(table->philos[i]))
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
	sim_start_delay(table->start_time);
	set_sim_stop_flag(table, false);
	while (true)
	{
		if (end_condition_reached(table) == true)
			return (NULL);
	}
	return (NULL);
}
