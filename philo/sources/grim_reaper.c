/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grim_reaper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 12:00:18 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/04 13:20:20 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	philos_all_ate_enough(t_table *table)
{
	unsigned int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		if ((int)table->philos[i]->times_ate < table->must_eat_count)
			return (false);
		i++;
	}
	return (true);
}

void	*grim_reaper(void *data)
{
	t_philo	*philo;
	t_table	*t;

	philo = (t_philo *)data;
	t = philo->table;
	while (t->all_alive)
	{
		if (!philo->is_eating
			&& (get_time_in_ms() - philo->last_meal >= t->time_to_die))
		{
			pthread_mutex_lock(&philo->eat_lock);
			write_status(t, philo->id, "died", RED);
			t->all_alive = 0;
			pthread_mutex_unlock(&philo->eat_lock);
		}
		if ((int)philo->times_ate == t->must_eat_count && philos_all_ate_enough(t))
			t->all_alive = 0;
		usleep(100);
	}
	return (NULL);
}
