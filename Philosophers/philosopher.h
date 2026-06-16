/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:40:56 by jbias             #+#    #+#             */
/*   Updated: 2025/04/10 14:50:55 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <stdarg.h>

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	last_eat_mutex;
	int				last_eat;
	int				eat_count;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	int				philos_count;
	int				death_timer;
	int				eat_timer;
	int				sleep_timer;
	int				max_eat;
	int				start_time;
	bool			is_dead;
	pthread_mutex_t	*forks;
	pthread_mutex_t	is_dead_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	t_philo			*philos;
}	t_data;

void		init(t_data *data, char **av);
void		init_philos(t_data *data);
void		free_all(t_data *data);
void		print_status(t_philo *philo, char *status);
int			eat(t_philo *philo);
void		*routine(void *arg);
void		*check(void *arg);
int			ft_putstr(const char *str);
int			get_time(void);
void		forks(t_philo *philo);
void		unlock_forks(t_philo *philo);
void		check2(t_data *data);
void		create_threads(t_data *data);
int			join_threads(t_data *data);
void		*sleep_and_think(t_philo *philo);
void		routine_delay(t_philo *philo);

#endif
