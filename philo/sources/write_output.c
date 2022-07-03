/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 13:20:23 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/03 12:36:36 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	write_status(t_table *t, int id, char *str, char *color)
{
	pthread_mutex_lock(&t->write_lock);
	printf("[%ld]\t%sPhilo #%d %s\e[0m\n",
		get_time_in_ms() - t->start_time, color, id + 1, str);
	if (str[0] != 'd')
		pthread_mutex_unlock(&t->write_lock);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	exit_error(char *str, t_table *table)
{
	if (table != NULL)
		free_table(table);
	error_msg(str);
	return (EXIT_FAILURE);
}

void	error_msg(char *str)
{
	write(2, "philo: ", 7);
	write(2, str, ft_strlen(str));
}

void	write_outcome(t_table *table)
{
	int	i;
	int	full_count;

	full_count = 0;
	i = 0;
	while (i < table->nb_philos)
	{
		if (table->philos[i]->times_ate == table->must_eat_count)
		{
			printf("philo %d ate %d times.\n", i + 1, table->philos[i]->times_ate);
			full_count++;
		}
		i++;
	}
	printf("%d/%d philosophers ate %d times.\n",
		full_count, table->nb_philos, table->must_eat_count);
}

int	exit_usage(void)
{
	printf("philo: usage: ./philo [number_of_philosophers] \
[time_to_die] [time_to_eat] [time_to_sleep] \
[number_of_times_each_philosopher_must_eat]\n");
	return (EXIT_SUCCESS);
}
