/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 11:46:01 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/04 11:11:26 by mcombeau         ###   ########.fr       */
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

# define MAX_PHILOS 500

/*	Macros	*/

# define NC "\e[0m"
# define RED "\e[31m"
# define GREEN "\e[32m"
# define PURPLE "\e[35m"
# define CYAN "\e[36m"

# define STR_STATUS	"[%10ld]\t%sPhilo #%d %s\e[0m\n"
# define STR_FORK	"has taken a fork"
# define STR_EAT	"is eating"
# define STR_SLEEP	"is sleeping"
# define STR_THINK	"is thinking"
# define STR_DIE	"died"

# define STR_PROG_NAME	"philo:"
# define STR_ERR_THREAD	"%s error: Could not create thread.\n"
# define STR_ERR_MALLOC	"%s error: Could not allocate memory.\n"
# define STR_ERR_MUTEX	"%s error: Could not create mutex.\n"
# define STR_ERR_INPUT_DIGIT	"%s invalid input: %s: \
not a valid positive unsigned integer between 0 and 2147483647.\n"
# define STR_ERR_INPUT_VAL_OFLOW	"%s invalid input: %s: \
value overflow, max = %u\n"
# define STR_ERR_INPUT_PHILO_OFLOW	"%s invalid input: %s: \
philosopher overflow, max = %u\n"

# define STR_USAGE "%s usage: ./philo [number_of_philosophers] \
[time_to_die] [time_to_eat] [time_to_sleep] \
(number_of_times_each_philosopher_must_eat)\n"

/*	Structures	*/
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
	pthread_mutex_t		write_lock;
	pthread_mutex_t		*fork_locks;
	t_philo				**philos;
}	t_table;

/* Functions */
int					is_valid_input(int ac, char **av);
int					integer_atoi(char *str);
int					exit_error(char *str, t_table *table);
int					msg(char *str, char *detail, int exit_no);
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