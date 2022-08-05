/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 17:27:50 by mcombeau          #+#    #+#             */
/*   Updated: 2022/08/05 16:44:16 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* msg:
*	Writes a message to the console. Returns the provided exit number.
*	Used for error management.
*/
int	msg(char *str, char *detail, int exit_no)
{
	if (!detail)
		printf(str, STR_PROG_NAME);
	else
		printf(str, STR_PROG_NAME, detail);
	return (exit_no);
}

/* error_failure:
*	Frees any allocated memory, prints an error message and
*	returns 0 to indicate failure.
*	Used for error management during initialization.
*/
int	error_failure(char *str, char *details, t_table *table)
{
	if (table != NULL)
		free_table(table);
	return (msg(str, details, 0));
}

/* error_null:
*	Frees any allocated memory, prints an error message and returns a NULL pointer.
*	Used for error management during initialization.
*/
void	*error_null(char *str, char *details, t_table *table)
{
	if (table != NULL)
		free_table(table);
	msg(str, details, EXIT_FAILURE);
	return (NULL);
}

/* table_cleanup:
*	Closes all semaphores and frees any allocated memory.
*	Returns the provided exit code.
*/
int	table_cleanup(t_table *table, int exit_code)
{
	if (table != NULL)
	{
		sem_close(table->sem_forks);
		sem_close(table->sem_write);
		sem_unlink(SEM_NAME_FORKS);
		sem_unlink(SEM_NAME_WRITE);
		free_table(table);
	}
	return (exit_code);
}

/* child_exit:
*	Exits a child philosopher process with the appropriate exit code.
*	Prints an error message if the child encountered an error.
*/
void	child_exit(t_table *table, int exit_code)
{
	sem_post(table->this_philo->sem_meal);
	if (exit_code == CHILD_EXIT_ERR_SEM)
		msg(STR_ERR_SEM, NULL, 0);
	if (exit_code == CHILD_EXIT_ERR_PTHREAD)
		msg(STR_ERR_THREAD, NULL, 0);
	sem_close(table->this_philo->sem_meal);
//	sem_close(table->this_philo->sem_forks);
//	sem_close(table->this_philo->sem_write);
//	free_table(table);
	exit(exit_code);
}
