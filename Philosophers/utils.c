/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:41:02 by jbias             #+#    #+#             */
/*   Updated: 2025/04/10 14:46:31 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	free_all(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos_count)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].last_eat_mutex);
		i++;
	}
	free(data->forks);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	pthread_mutex_destroy(&data->is_dead_mutex);
	free(data->philos);
}

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	pthread_mutex_lock(&philo->data->is_dead_mutex);
	if (!philo->data->is_dead)
		printf("%d %d %s\n", get_time() - philo->data->start_time,
			philo->id, status);
	pthread_mutex_unlock(&philo->data->is_dead_mutex);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

int	eat(t_philo *philo)
{
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->last_eat_mutex);
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->last_eat_mutex);
	usleep(philo->data->eat_timer * 1000);
	philo->eat_count++;
	unlock_forks(philo);
	if (philo->data->max_eat > 0 && philo->eat_count >= philo->data->max_eat)
		return (1);
	return (0);
}

void	*check(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		check2(data);
		pthread_mutex_lock(&data->is_dead_mutex);
		if (data->is_dead == true)
			return (pthread_mutex_unlock(&data->is_dead_mutex), NULL);
		pthread_mutex_unlock(&data->is_dead_mutex);
		usleep(100);
	}
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	routine_delay(philo);
	while (1)
	{
		pthread_mutex_lock(&philo->data->is_dead_mutex);
		if (philo->data->is_dead)
			return (pthread_mutex_unlock(&philo->data->is_dead_mutex), NULL);
		pthread_mutex_unlock(&philo->data->is_dead_mutex);
		forks(philo);
		pthread_mutex_lock(&philo->data->is_dead_mutex);
		if (philo->data->is_dead)
		{
			unlock_forks(philo);
			return (pthread_mutex_unlock(&philo->data->is_dead_mutex), NULL);
		}
		pthread_mutex_unlock(&philo->data->is_dead_mutex);
		if (eat(philo) == 1)
			return (NULL);
		sleep_and_think(philo);
	}
	return (NULL);
}
