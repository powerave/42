/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:00:07 by jbias             #+#    #+#             */
/*   Updated: 2025/02/11 14:03:10 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

int	move_up_end(t_data *data)
{
	int	x;
	int	y;

	x = data->game.x;
	y = data->game.y;
	if (data->game.map[x - 1][y] != '1')
	{
		if (data->game.map[x - 1][y] == 'E')
		{
			move_player(data, x - 1, y);
			data->game.x--;
			data->game.steps++;
			ft_printf("end steps : %d\n", data->game.steps);
			victory(data);
		}
		move_player(data, x - 1, y);
		data->game.x--;
		data->game.steps++;
		ft_printf("moved up --> steps : %d\n", data->game.steps);
	}
	return (0);
}

int	move_down_end(t_data *data)
{
	int	x;
	int	y;

	x = data->game.x;
	y = data->game.y;
	if (data->game.map[x + 1][y] != '1')
	{
		if (data->game.map[x + 1][y] == 'E')
		{
			move_player(data, x + 1, y);
			data->game.x++;
			data->game.steps++;
			ft_printf("end steps : %d\n", data->game.steps);
			victory(data);
		}
		move_player(data, x + 1, y);
		data->game.x++;
		data->game.steps++;
		ft_printf("moved down --> steps : %d\n", data->game.steps);
	}
	return (0);
}

int	move_left_end(t_data *data)
{
	int	x;
	int	y;

	x = data->game.x;
	y = data->game.y;
	if (data->game.map[x][y - 1] != '1')
	{
		if (data->game.map[x][y - 1] == 'E')
		{
			move_player(data, x, y - 1);
			data->game.y--;
			data->game.steps++;
			ft_printf("end steps : %d\n", data->game.steps);
			victory(data);
		}
		move_player(data, x, y - 1);
		data->game.y--;
		data->game.steps++;
		ft_printf("moved left --> steps : %d\n", data->game.steps);
	}
	return (0);
}

int	move_right_end(t_data *data)
{
	int	x;
	int	y;

	x = data->game.x;
	y = data->game.y;
	if (data->game.map[x][y + 1] != '1')
	{
		if (data->game.map[x][y + 1] == 'E')
		{
			move_player(data, x, y + 1);
			data->game.y++;
			data->game.steps++;
			ft_printf("end steps : %d\n", data->game.steps);
			victory(data);
		}
		move_player(data, x, y + 1);
		data->game.y++;
		data->game.steps++;
		ft_printf("moved right --> steps : %d\n", data->game.steps);
	}
	return (0);
}

int	move_end(int key, t_data *data)
{
	if (key == ESC)
		return (quit(data), 0);
	if (key == DOWN)
		return (move_down_end(data));
	if (key == UP)
		return (move_up_end(data));
	if (key == LEFT)
		return (move_left_end(data));
	if (key == RIGHT)
		return (move_right_end(data));
	else
		return (0);
}
