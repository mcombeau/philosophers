/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 11:46:06 by mcombeau          #+#    #+#             */
/*   Updated: 2022/08/05 11:32:15 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* has_simulation_stopped:
*	TODO
*/
bool	has_simulation_stopped(t_table *table)
{
	unsigned int	i;
	bool			r;

	r = true;
	i = 0;
	while (i < table->nb_philos)
	{
		if (table->pids[i] != 0)
			r = false;
		i++;
	}
	return (r);
}

/* start_simulation:
*	Launches the simulation by creating a grim reaper thread as well as
*	one thread for each philosopher.
*	Returns true if the simulation was successfully started, false if there
*	was an error. 
*/
static bool	start_simulation(t_table *table)
{
	unsigned int	i;
	pid_t			pid;

	table->start_time = get_time_in_ms() + 100 + table->nb_philos * 2;
//	table->start_time = get_time_in_ms() + table->nb_philos;
	i = 0;
	while (i < table->nb_philos)
	{
		pid = fork();
		if (pid == -1)
			return (error_failure(STR_ERR_FORK, NULL, table));
		else if (pid > 0)
		{
			table->pids[i] = pid;
			table->philos[i]->pid = pid;
		}
		else if (pid == 0)
		{
			table->this_philo = table->philos[i];
			philosopher(table);
		}
		i++;
	}
	sim_start_delay(table->start_time);
	return (true);
}

//TODO
static int	get_child_philo(t_table *table, pid_t *pid)
{
	int	philo_exit_code;
	int	exit_code;

	if (*pid && waitpid(*pid, &philo_exit_code, WNOHANG) != 0)
	{
		if (WIFEXITED(philo_exit_code))
		{
			exit_code = WEXITSTATUS(philo_exit_code);
			if (exit_code == CHILD_EXIT_PHILO_DEAD)
				return (kill_all_philos(table, 1));
			if (exit_code == CHILD_EXIT_PHILO_FULL)
			{
				table->full_count += 1;
				*pid = 0;
			}
			if (exit_code == CHILD_EXIT_ERR_PTHREAD ||
				exit_code == CHILD_EXIT_ERR_SEM)
				return (kill_all_philos(table, -1));
		}
	}
	return (0);
}

/* stop_simulation:
*	TODO.
*/
static int	stop_simulation(t_table	*table)
{
	unsigned int	i;
	int				exit_code;

	while (!has_simulation_stopped(table))
	{
		i = 0;
		while (i < table->nb_philos)
		{
			exit_code = get_child_philo(table, &table->pids[i]);
			if (exit_code == 1 || exit_code == -1)
				return (exit_code);
			i++;
		}
	}
	if (table->must_eat_count >= 0)
		write_outcome(table);
	return (0);
}

int	main(int ac, char **av)
{
	t_table	*table;

	table = NULL;
	if (ac - 1 < 4 || ac - 1 > 5)
		return (msg(STR_USAGE, NULL, EXIT_FAILURE));
	if (!is_valid_input(ac, av))
		return (EXIT_FAILURE);
	table = init_table(ac, av, 1);
	if (!table)
		return (EXIT_FAILURE);
	if (!start_simulation(table))
		return (EXIT_FAILURE);
	if (stop_simulation(table) == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
