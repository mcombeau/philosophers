/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 11:46:01 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/04 17:43:40 by mcombeau         ###   ########.fr       */
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
# include <stdbool.h>

/*------------------------------------------------------*
*						Macros							*
*-------------------------------------------------------*/

# define MAX_PHILOS	1000
# define STR_MAX_PHILOS "1000"

# define NC		"\e[0m"
# define RED	"\e[31m"
# define GREEN	"\e[32m"
# define PURPLE	"\e[35m"
# define CYAN	"\e[36m"

# define STR_STATUS	"[%10ld]\t%sPhilo #%d %s\e[0m\n"
# define STR_FORK	"has taken a fork"
# define STR_EAT	"is eating"
# define STR_SLEEP	"is sleeping"
# define STR_THINK	"is thinking"
# define STR_DIE	"died"

# define STR_PROG_NAME	"philo:"

# define STR_USAGE	"%s usage: ./philo [number_of_philosophers] \
[time_to_die] [time_to_eat] [time_to_sleep] \
(number_of_times_each_philosopher_must_eat)\n"
# define STR_ERR_INPUT_DIGIT	"%s invalid input: %s: \
not a valid positive unsigned integer between 1 and 2147483647.\n"
# define STR_ERR_INPUT_POFLOW	"%s invalid input: \
too many philosophers, max: %s\n"
# define STR_ERR_THREAD	"%s error: Could not create thread.\n"
# define STR_ERR_MALLOC	"%s error: Could not allocate memory.\n"
# define STR_ERR_MUTEX	"%s error: Could not create mutex.\n"

/*------------------------------------------------------*
*						Structures						*
*-------------------------------------------------------*/

typedef struct s_philo	t_philo;

typedef struct s_table
{
	time_t			start_time;
	unsigned int	nb_philos;
	pthread_t		grim_reaper;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	unsigned int	must_eat_count;
	bool			sim_stop;
	pthread_mutex_t	sim_stop_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	*fork_locks;
	t_philo			**philos;
}	t_table;

typedef struct s_philo
{
	pthread_t			thread;
	unsigned int		id;
	unsigned int		times_ate;
	unsigned int		left_fork;
	unsigned int		right_fork;
	pthread_mutex_t		death_lock;
	time_t				last_meal;
	t_table				*table;
}	t_philo;

/*------------------------------------------------------*
*					Function Prototypes					*
*-------------------------------------------------------*/

//	parsing.c
bool			is_valid_input(int ac, char **av);
int				integer_atoi(char *str);

//	init.c
t_table			*init_table(int ac, char **av, int i);

//	routines.c
void			*philosopher(void *data);

//	time.c
time_t			get_time_in_ms(void);
void			philo_sleep(t_table *table, time_t sleep_time);
void			sim_start_delay(time_t start_time);

//	output.c
void			write_status(t_table *t, int id, char *str, char *color);
void			write_outcome(t_table *table);
void			*error_msg(char *str, char *details, t_table *table);
int				msg(char *str, char *detail, int exit_no);

//	grim_reaper.c
void			*grim_reaper(void *data);
bool			has_simulation_stopped(t_table *table);

//	exit.c
int				exit_error(char *str, char *details, t_table *table);
void			*free_table(t_table *table);
void			destroy_mutexes(t_table *table);

#endif