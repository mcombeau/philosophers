/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 11:46:06 by mcombeau          #+#    #+#             */
/*   Updated: 2022/09/11 13:53:52 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* has_simulation_stopped:
*	Checks if the simulation is about to end.
*	Returns true if the simulation must stop, false if not.
*/
bool	has_simulation_stopped(t_table *table)
{
	bool	ret;

	sem_wait(table->sem_stop);
	ret = table->stop_sim;
	sem_post(table->sem_stop);
	return (ret);
}

/* start_simulation:
*	Launches the simulation by creating a new child process for each
*	philosopher. The process ids are recorded to be able to wait for each
*	child in turn, and send them a kill signal if need be.
*	Also creates a grim reaper thread to monitor philosophers and detect
*	if everyone has eaten enough.
*
*	Returns true if the simulation was successfully started, false if there
*	was an error.
*/
static bool	start_simulation(t_table *table)
{
	unsigned int	i;
	pid_t			pid;

	table->start_time = get_time_in_ms() + ((table->nb_philos * 2) * 10);
	i = -1;
	while (++i < table->nb_philos)
	{
		pid = fork();
		if (pid == -1)
			return (error_failure(STR_ERR_FORK, NULL, table));
		else if (pid > 0)
			table->pids[i] = pid;
		else if (pid == 0)
		{
			table->this_philo = table->philos[i];
			philosopher(table);
		}
	}
	if (start_grim_reaper_threads(table) == false)
		return (false);
	return (true);
}

/* get_child_philo:
*	Waits for a philosopher process to exit. If the philo process
*	exits with an error or a dead philosopher, sends the signal to
*	kill all other child processes.
*/
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
			if (exit_code == CHILD_EXIT_ERR_PTHREAD
				|| exit_code == CHILD_EXIT_ERR_SEM)
				return (kill_all_philos(table, -1));
			if (exit_code == CHILD_EXIT_PHILO_FULL)
			{
				table->philo_full_count += 1;
				return (1);
			}
		}
	}
	return (0);
}

/* stop_simulation:
*	Waits for each child process and analyses its exit code
*	as long as the simulation is still ongoing.
*	Ends the simulation when one of the end conditions are fulfilled:
*	when a philosopher dies or when all philosophers have eaten enough.
*/
static int	stop_simulation(t_table	*table)
{
	unsigned int	i;
	int				exit_code;

	sim_start_delay(table->start_time);
	while (has_simulation_stopped(table) == false)
	{
		i = 0;
		while (i < table->nb_philos)
		{
			exit_code = get_child_philo(table, &table->pids[i]);
			if (exit_code == 1 || exit_code == -1)
			{
				sem_wait(table->sem_stop);
				table->stop_sim = true;
				sem_post(table->sem_philo_full);
				sem_post(table->sem_philo_dead);
				sem_post(table->sem_stop);
				return (exit_code);
			}
			i++;
		}
	}
	return (0);
}

/* main:
*	Main philosophers function.
*/
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
		return (table_cleanup(table, EXIT_FAILURE));
	if (DEBUG_FORMATTING == 1 && table->must_eat_count >= 0)
		write_outcome(table);
	return (table_cleanup(table, EXIT_SUCCESS));
}
