/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ipc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 16:38:33 by mcombeau          #+#    #+#             */
/*   Updated: 2022/08/06 11:25:01 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

/* init_philo_ipc:
*	Initializes interprocess communication between philosopher processes.
*	Each philosopher opens the same named fork and write semaphores. These
*	semaphores are stored in shared memory and are not duplicated for each
*	process. Named semaphores are stored in /dev/shm on Linux systems and
*	behave similarly to files on disk.
*
*	Each philosopher process also creates its own grim reaper thread which
*	detects if the philosopher is dead or has eaten enough.
*/
void	init_philo_ipc(t_table *table, t_philo *philo)
{
	if (table->nb_philos == 1)
		return ;
	sem_unlink(philo->sem_meal_name);
	philo->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT,
			S_IRUSR | S_IWUSR, table->nb_philos);
	if (philo->sem_forks == SEM_FAILED)
		child_exit(table, CHILD_EXIT_ERR_SEM);
	philo->sem_write = sem_open(SEM_NAME_WRITE, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (philo->sem_write == SEM_FAILED)
		child_exit(table, CHILD_EXIT_ERR_SEM);
	philo->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, table->nb_philos);
	if (philo->sem_philo_full == SEM_FAILED)
		child_exit(table, CHILD_EXIT_ERR_SEM);
	philo->sem_meal = sem_open(philo->sem_meal_name, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (philo->sem_meal == SEM_FAILED)
		child_exit(table, CHILD_EXIT_ERR_SEM);
	sem_unlink(philo->sem_meal_name);
	if (pthread_create(&philo->personal_grim_reaper, NULL,
			&personal_grim_reaper, table) != 0)
		child_exit(table, CHILD_EXIT_ERR_PTHREAD);
	pthread_detach(philo->personal_grim_reaper);
	return ;
}
