/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:40:50 by jbias             #+#    #+#             */
/*   Updated: 2025/04/10 14:40:52 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int	lock_forks(t_philo *p)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (p->left_fork < p->right_fork)
	{
		first = p->left_fork;
		second = p->right_fork;
	}
	else
	{
		first = p->right_fork;
		second = p->left_fork;
	}
	pthread_mutex_lock(first);
	print_status(p, "has taken a fork");
	pthread_mutex_lock(second);
	print_status(p, "has taken a fork");
	return (0);
}

void	unlock_forks(t_philo *p)
{
	if (p->left_fork < p->right_fork)
	{
		pthread_mutex_unlock(p->right_fork);
		pthread_mutex_unlock(p->left_fork);
	}
	else
	{
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);
	}
}

void	forks(t_philo *philo)
{
	if (philo->data->philos_count == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		while (!philo->data->is_dead)
			usleep(1000);
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	lock_forks(philo);
}

static int	all_ate_n_times(t_data *data)
{
	int	i;
	int	count;

	if (data->max_eat <= 0)
		return (0);
	i = 0;
	count = 0;
	while (i < data->philos_count)
	{
		pthread_mutex_lock(&data->philos[i].last_eat_mutex);
		if (data->philos[i].eat_count >= data->max_eat)
			count++;
		pthread_mutex_unlock(&data->philos[i].last_eat_mutex);
		i++;
	}
	if (count == data->philos_count)
	{
		pthread_mutex_lock(&data->is_dead_mutex);
		data->is_dead = true;
		pthread_mutex_unlock(&data->is_dead_mutex);
		printf("all philosophers have eaten enough\n");
		return (1);
	}
	return (0);
}

void	check2(t_data *data)
{
	int	i;

	if (all_ate_n_times(data) == 1)
		return ;
	i = 0;
	while (i < data->philos_count)
	{
		pthread_mutex_lock(&data->philos[i].last_eat_mutex);
		if ((get_time() - data->philos[i].last_eat) > data->death_timer)
		{
			pthread_mutex_unlock(&data->philos[i].last_eat_mutex);
			print_status(&data->philos[i], "died");
			pthread_mutex_lock(&data->is_dead_mutex);
			data->is_dead = true;
			pthread_mutex_unlock(&data->is_dead_mutex);
			return ;
		}
		pthread_mutex_unlock(&data->philos[i++].last_eat_mutex);
	}
}
