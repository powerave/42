/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 18:58:01 by jbias             #+#    #+#             */
/*   Updated: 2025/06/04 18:58:07 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	*sleep_and_think(t_philo *philo)
{	
	pthread_mutex_lock(&philo->data->is_dead_mutex);
	if (philo->data->is_dead)
		return (pthread_mutex_unlock(&philo->data->is_dead_mutex), NULL);
	pthread_mutex_unlock(&philo->data->is_dead_mutex);
	print_status(philo, "is sleeping");
	usleep(philo->data->sleep_timer * 1000);
	pthread_mutex_lock(&philo->data->is_dead_mutex);
	if (philo->data->is_dead)
		return (pthread_mutex_unlock(&philo->data->is_dead_mutex), NULL);
	pthread_mutex_unlock(&philo->data->is_dead_mutex);
	print_status(philo, "is thinking");
	usleep(1000);
	return (NULL);
}

void	routine_delay(t_philo *philo)
{
	if (philo->data->philos_count % 2 == 1)
	{
		if (philo->id % 2 == 0)
			usleep(1000);
	}
	else
	{
		if (philo->id % 2 == 0)
			usleep(1000);
	}
}

void	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos_count)
	{
		pthread_create(&data->philos[i].thread, NULL, routine,
			&data->philos[i]);
		i++;
	}
}

int	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos_count)
	{
		if (pthread_join(data->philos[i].thread, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	ft_putstr(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		str = "(null)";
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
	return (i);
}
