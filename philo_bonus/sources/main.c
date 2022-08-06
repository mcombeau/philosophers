/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 11:46:06 by mcombeau          #+#    #+#             */
/*   Updated: 2022/08/06 12:56:35 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* has_simulation_stopped:
*	Checks if the simulation is about to end.
*	Returns true if none of the process IDs were set to 0
*	(meaning they ended with a simulation-stopping exit code)
*	or false if none of the processes finished yet.
*/
bool	has_simulation_stopped(t_table *table)
{
	unsigned int	i;
	bool			r;

	r = true;
	sem_wait(table->sem_all_ate_enough);
	if (table->all_philos_full == true)
	{
		sem_post(table->sem_all_ate_enough);
		return (r);
	}
	sem_post(table->sem_all_ate_enough);
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
*	Launches the simulation by creating a new child process for each
*	philosopher. The process ids are recorded to be able to wait for each
*	child in turn, and send them a kill signal if need be.
*	Returns true if the simulation was successfully started, false if there
*	was an error.
*/
static bool	start_simulation(t_table *table)
{
	unsigned int	i;
	pid_t			pid;

	table->start_time = get_time_in_ms() + 100 + table->nb_philos * 2;
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
	if (pthread_create(&table->grim_reaper, NULL,
			&global_grim_reaper, table) != 0)
		return (error_failure(STR_ERR_THREAD, NULL, table));
	pthread_detach(table->grim_reaper);
	return (true);
}

/* get_child_philo:
*	Waits for a philosopher process to exit. If the philo process
*	exits with an error or a dead philosopher, sends the signal to
*	kill all other child processes. If the philosopher process
*	exited because the philosopher ate enough meals, sets the 
*	process ID to 0 and increments the number of full philosophers
*	by one to display a summary at the end of the simulation.
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
				return (exit_code);
			i++;
		}
	}
	if (table->must_eat_count >= 0)
		write_outcome(table);
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
	return (table_cleanup(table, EXIT_SUCCESS));
}
