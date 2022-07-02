/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 13:20:23 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/02 16:22:51 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	write_status(t_table *table, int id, char *str)
{
	pthread_mutex_lock(&table->write_lock);
	printf("%ld %d %s", get_time_in_ms() - table->start_time, id + 1, str);
	if (str[0] != 'd')
		pthread_mutex_unlock(&table->write_lock);
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

void	*free_table(t_table *table)
{
	int	i;

	if (!table)
		return (NULL);
	if (table->fork_locks != NULL)
		free(table->fork_locks);
	if (table->philos != NULL)
	{
		i = 0;
		while (i < table->nb_philos)
		{
			if (table->philos[i] != NULL)
				free(table->philos[i]);
			i++;
		}
		free(table->philos);
	}
	free(table);
	return (NULL);
}

void	error_msg(char *str)
{
	write(2, "philo: ", 7);
	write(2, str, ft_strlen(str));
}

int	exit_usage(void)
{
	printf("philo: usage: ./philo [number_of_philosophers] \
[time_to_die] [time_to_eat] [time_to_sleep] \
[number_of_times_each_philosopher_must_eat]\n");
	return (EXIT_SUCCESS);
}
