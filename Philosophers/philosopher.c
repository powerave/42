/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:40:41 by jbias             #+#    #+#             */
/*   Updated: 2025/04/10 14:43:55 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-')
		return (-1);
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res);
}

int	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	init(t_data *data, char **av)
{
	int	i;

	i = 0;
	data->philos_count = ft_atoi(av[1]);
	data->death_timer = ft_atoi(av[2]);
	data->eat_timer = ft_atoi(av[3]);
	data->sleep_timer = ft_atoi(av[4]);
	data->max_eat = -1;
	if (av[5])
		data->max_eat = ft_atoi(av[5]);
	data->start_time = get_time();
	data->is_dead = false;
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->death_mutex, NULL);
	pthread_mutex_init(&data->is_dead_mutex, NULL);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philos_count);
	if (!data->forks)
		return ;
	while (i < data->philos_count)
		pthread_mutex_init(&data->forks[i++], NULL);
	data->philos = malloc(sizeof(t_philo) * data->philos_count);
	if (!data->philos)
		return ;
	init_philos(data);
}

void	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos_count)
	{
		data->philos[i].data = data;
		data->philos[i].id = i + 1;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->philos_count];
		data->philos[i].last_eat = data->start_time;
		data->philos[i].eat_count = 0;
		pthread_mutex_init(&data->philos[i].last_eat_mutex, NULL);
		i++;
	}
}

int	main(int ac, char **av)
{
	t_data		data;
	pthread_t	checker;

	if (ac != 5 && ac != 6)
		return (ft_putstr("Error: wrong number of arguments\n"), 1);
	init(&data, av);
	if (data.philos_count < 1 || data.death_timer < 0
		|| data.eat_timer < 0 || data.sleep_timer < 0
		|| (ac == 6 && data.max_eat < 0))
		return (ft_putstr("Error: invalid arguments\n"), free_all(&data), 1);
	create_threads(&data);
	if (pthread_create(&checker, NULL, check, &data) != 0)
		return (free_all(&data), ft_putstr("Error: pthread_create\n"), 1);
	pthread_join(checker, NULL);
	if (join_threads(&data) == 1)
		return (free_all(&data), ft_putstr("Error: pthread_join\n"), 1);
	free_all(&data);
	return (0);
}
