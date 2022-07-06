/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 13:20:23 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/06 14:30:14 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_status(t_table *t, int id, char *color, char *str)
{
	pthread_mutex_lock(&t->write_lock);
	if (has_simulation_stopped(t) == false)
		printf("[%10ld]\t%sPhilo #%d %s\e[0m\n", get_time_in_ms() - t->start_time,
			color, id + 1, str);
	pthread_mutex_unlock(&t->write_lock);
}

void	write_status(t_table *table, int id, t_status status)
{
	if (status == DIED)
		print_status(table, id, RED, "died");
	else if (status == GOT_FORK)
		print_status(table, id, PURPLE, "has taken a fork");
	else if (status == EATING)
		print_status(table, id, GREEN, "is eating");
	else if (status == SLEEPING)
		print_status(table, id, CYAN, "is sleeping");
	else if (status == THINKING)
		print_status(table, id, CYAN, "is thinking");
}

void	*error_msg(char *str, char *details, t_table *table)
{
	if (table != NULL)
		free_table(table);
	msg(str, details, EXIT_FAILURE);
	return (NULL);
}

void	write_outcome(t_table *table)
{
	unsigned int	i;
	unsigned int	full_count;

	full_count = 0;
	i = 0;
	while (i < table->nb_philos)
	{
		if (table->philos[i]->times_ate >= table->must_eat_count)
			full_count++;
		i++;
	}
	pthread_mutex_lock(&table->write_lock);
	printf("%d/%d philosophers ate at least %d times.\n",
		full_count, table->nb_philos, table->must_eat_count);
	pthread_mutex_unlock(&table->write_lock);
	return ;
}

int	msg(char *str, char *detail, int exit_no)
{
	if (!detail)
		printf(str, STR_PROG_NAME);
	else
		printf(str, STR_PROG_NAME, detail);
	return (exit_no);
}
