/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ipc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 16:38:33 by mcombeau          #+#    #+#             */
/*   Updated: 2022/08/04 18:00:20 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

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