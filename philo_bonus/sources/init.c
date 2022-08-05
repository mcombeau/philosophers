/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 11:35:04 by mcombeau          #+#    #+#             */
/*   Updated: 2022/08/05 15:51:07 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* set_meal_sem_name:
*	Creates a unique semaphore name to create a mutex that protects
*	a philosopher's own meal variables. The name must be unique, otherwise
*	all philosopher processes would share the same meal semaphore.
*	In order to create this semaphore name, simply concatenate the standard
*	semaphore meal name with the philosopher's ID.
*	Returns the unique semaphore name with the given ID. NULL if memory
*	allocation fails.
*/
static char	*set_meal_sem_name(unsigned int id)
{
	unsigned int	i;
	unsigned int	digit_count;
	char			*sem_name;
	char			*tmp;

	digit_count = 0;
	i = id;
	while (i)
	{
		digit_count++;
		i /= 10;
	}
	tmp = ft_utoa(id, digit_count);
	if (tmp == NULL)
		return (NULL);
	i = ft_strlen(SEM_NAME_MEAL) + digit_count;
	sem_name = malloc (sizeof * sem_name * (i + 1));
	if (sem_name == NULL)
	{
		free(tmp);
		return (NULL);
	}
	sem_name[0] = '\0';
	sem_name = ft_strcat(sem_name, SEM_NAME_MEAL);
	sem_name = ft_strcat(sem_name, tmp);
	free(tmp);
	return (sem_name);
}

/* init_philosophers:
*	Allocates memory for each philosopher and initializes their values.
*	Also creates specific semaphore names to protect their own meal-related
*	variables. The semaphore names for sem_meal must be unique to each
*	philosopher because the semaphore shouldn't be accessed by a different
*	philosopher process.
*	Returns a pointer to the array of philosophers or NULL if
*	initialization failed.
*/
static t_philo	**init_philosophers(t_table *table)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo) * (table->nb_philos + 1));
	if (!philos)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < table->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (error_null(STR_ERR_MALLOC, NULL, 0));
		philos[i]->sem_meal_name = set_meal_sem_name(i + 1);
		if (philos[i]->sem_meal_name == NULL)
			return (error_null(STR_ERR_MALLOC, NULL, 0));
		philos[i]->table = table;
		philos[i]->id = i;
		philos[i]->times_ate = 0;
		philos[i]->nb_forks_held = 0;
		i++;
	}
	return (philos);
}

/* init_global_semaphores:
*	Initializes semaphores for forks and writing. These sempahores are
*	first opened in the parent process, but each child process will open
*	the same named semaphores, which will allow interprocess communication
*	through these semaphores.
*	Returns true if the initalizations were successful, false if
*	initilization failed.
*/
static bool	init_global_semaphores(t_table *table)
{
	sem_unlink(SEM_NAME_FORKS);
	sem_unlink(SEM_NAME_WRITE);
	table->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT,
								S_IRUSR | S_IWUSR, table->nb_philos);
	if (table->sem_forks == SEM_FAILED)
		return (error_failure(STR_ERR_SEM, NULL, table));
	table->sem_write = sem_open(SEM_NAME_WRITE, O_CREAT,
							S_IRUSR | S_IWUSR, 1);
	if (table->sem_write == SEM_FAILED)
	{
		sem_close(table->sem_forks);
		sem_unlink(SEM_NAME_FORKS);
		return (error_failure(STR_ERR_SEM, NULL, table));
	}
	return (true);
}

/* init_table:
*	Initializes the "dining table", the data structure containing
*	all of the program's parameters.
*	Returns a pointer to the allocated table structure, or NULL if
*	an error occured during initialization.
*/
t_table	*init_table(int ac, char **av, int i)
{
	t_table	*table;

	table = malloc(sizeof(t_table) * 1);
	if (!table)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	table->nb_philos = integer_atoi(av[i++]);
	table->time_to_die = integer_atoi(av[i++]);
	table->time_to_eat = integer_atoi(av[i++]);
	table->time_to_sleep = integer_atoi(av[i++]);
	table->must_eat_count = -1;
	table->full_count = 0;
	if (ac - 1 == 5)
		table->must_eat_count = integer_atoi(av[i]);
	if (!init_global_semaphores(table))
		return (NULL);
	table->philos = init_philosophers(table);
	if (!table->philos)
		return (NULL);
	table->pids = malloc(sizeof * table->pids * table->nb_philos);
	if (!table->pids)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	return (table);
}
