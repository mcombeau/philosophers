/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ipc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 16:38:33 by mcombeau          #+#    #+#             */
/*   Updated: 2022/09/11 14:35:02 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

/* philo_open_global_semaphores:
*	Opens the semaphores shared between the parent and all child processes:
*		- Fork semaphore,
*		- Write semaphore,
*		- Full/philo ate enough semaphore.
*		- Dead/philo is dead semaphore.
*	Returns true if all semaphores were successfilly opened, false if
*	one semaphore failed to open.
*/
static bool	philo_open_global_semaphores(t_philo *philo)
{
	philo->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT,
			S_IRUSR | S_IWUSR, philo->table->nb_philos);
	if (philo->sem_forks == SEM_FAILED)
		return (false);
	philo->sem_write = sem_open(SEM_NAME_WRITE, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (philo->sem_write == SEM_FAILED)
		return (false);
	philo->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, philo->table->nb_philos);
	if (philo->sem_philo_full == SEM_FAILED)
		return (false);
	philo->sem_philo_dead = sem_open(SEM_NAME_DEAD, O_CREAT,
			S_IRUSR | S_IWUSR, philo->table->nb_philos);
	if (philo->sem_philo_dead == SEM_FAILED)
		return (false);
	return (true);
}

/* philo_open_local_semaphores:
*	Opens the semaphores used only by this child process. These semaphores
*	are not shared with the parent or any other child processes. They are used
*	to regulate only this process and its grim reaper threads access to certain
*	variables. They are:
*		- Meal semaphore (protects variables holding the philosophers last
*			meal time).
*		- Dead semaphore (so the grim reaper thread can interrupt the philosopher's
*			routine).
*	Returns true if all semaphores were successfilly opened, false if
*	one semaphore failed to open.
*/
static bool	philo_open_local_semaphores(t_philo *philo)
{
	philo->sem_meal = sem_open(philo->sem_meal_name, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (philo->sem_meal == SEM_FAILED)
		return (false);
	sem_unlink(philo->sem_meal_name);
	return (true);
}

/* init_philo_ipc:
*	Initializes interprocess communication between philosopher processes.
*	Each philosopher opens the same named semaphores. These	semaphores are 
*	stored in shared memory and are not duplicated for each process.
*	Named semaphores are stored in /dev/shm on Linux systems and behave
*	similarly to files on disk.
*
*	Each philosopher process also creates its own grim reaper thread which
*	detects if the philosopher is dead or has eaten enough.
*/
void	init_philo_ipc(t_table *table, t_philo *philo)
{
	if (table->nb_philos == 1)
		return ;
	sem_unlink(philo->sem_meal_name);
	if (!philo_open_global_semaphores(philo))
		child_exit(table, CHILD_EXIT_ERR_SEM);
	if (!philo_open_local_semaphores(philo))
		child_exit(table, CHILD_EXIT_ERR_SEM);
	if (pthread_create(&philo->personal_grim_reaper, NULL,
			&personal_grim_reaper, table) != 0)
		child_exit(table, CHILD_EXIT_ERR_PTHREAD);
	return ;
}
