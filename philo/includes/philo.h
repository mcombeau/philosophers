/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 11:46:01 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/03 12:35:52 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>

# define NC "\e[0m"
# define RED "\e[31m"
# define GREEN "\e[32m"
# define PURPLE "\e[35m"
# define CYAN "\e[36m"

typedef struct s_philo
{
	pthread_mutex_t		eat_lock;
	pthread_t			thread;
	pthread_t			grim_reaper;
	int					id;
	int					times_ate;
	int					left_fork;
	int					right_fork;
	int					is_eating;
	unsigned long int	last_meal;
	struct s_table		*table;
}	t_philo;

typedef struct s_table
{
	unsigned long int	start_time;
	int					nb_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					must_eat_count;
	int					all_alive;
	int					*philos_full;
	pthread_mutex_t		write_lock;
	pthread_mutex_t		*fork_locks;
	t_philo				**philos;
}	t_table;

int					is_valid_input(int ac, char **av);
int					integer_atoi(char *str);
int					exit_error(char *str, t_table *table);
void				error_msg(char *str);
int					exit_usage(void);
unsigned long int	get_time_in_ms(void);
void				*grim_reaper(void *data);
void				*philosopher(void *data);
void				*free_table(t_table *table);
void				write_status(t_table *t, int id, char *str, char *color);
void				write_outcome(t_table *table);

/*	Initialization	*/
t_table				*init_table(int ac, char **av, int i);
pthread_mutex_t		*init_forks(t_table *table);
t_philo				**init_philosophers(t_table *table);

#endif