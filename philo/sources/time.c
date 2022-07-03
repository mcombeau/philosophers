/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 14:07:22 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/03 14:06:55 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long int	get_time_in_ms(void)
{
	struct timeval		tv;
	unsigned long int	ms;

	gettimeofday(&tv, NULL);
	ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
//	printf("%ld:%ld = ms: %ld\n", tv.tv_sec, tv.tv_usec, ms);
	return (ms);
}

int	philos_all_ate_enough(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philos)
	{
		if (table->philos[i]->times_ate < table->must_eat_count)
			return (0);
		i++;
	}
	return (1);
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
		if (philo->times_ate == t->must_eat_count && philos_all_ate_enough(t))
			t->all_alive = 0;
		usleep(100);
	}
}
