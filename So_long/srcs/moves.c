/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 13:41:05 by jbias             #+#    #+#             */
/*   Updated: 2025/01/24 10:49:28 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

int	move_up(t_data *data)
{
	int	x;
	int	y;

	x = data->game.x;
	y = data->game.y;
	if (data->game.map[x - 1][y] != '1')
	{
		if (data->game.map[x - 1][y] == 'C')
		{
			data->game.collected++;
			data->game.map[x - 1][y] = '0';
		}
		exit_player_up(data, x, y);
		move_player(data, x - 1, y);
		data->game.x--;
		data->game.steps++;
		ft_printf("moved up --> steps : %d\n", data->game.steps);
	}
	return (0);
}

int	move_down(t_data *data)
{
	int	x;
	int	y;

	x = data->game.x;
	y = data->game.y;
	if (data->game.map[x + 1][y] != '1')
	{
		if (data->game.map[x + 1][y] == 'C')
		{
			data->game.collected++;
			data->game.map[x + 1][y] = '0';
		}
		exit_player_down(data, x, y);
		move_player(data, x + 1, y);
		data->game.x++;
		data->game.steps++;
		ft_printf("moved down --> steps : %d\n", data->game.steps);
	}
	return (0);
}

int	move_right(t_data *data)
{
	int	x;
	int	y;

	x = data->game.x;
	y = data->game.y;
	if (data->game.map[x][y + 1] != '1')
	{
		if (data->game.map[x][y + 1] == 'C')
		{
			data->game.collected++;
			data->game.map[x][y + 1] = '0';
		}
		exit_player_right(data, x, y);
		move_player(data, x, y + 1);
		data->game.y++;
		data->game.steps++;
		ft_printf("moved right --> steps : %d\n", data->game.steps);
	}
	return (0);
}

int	move_left(t_data *data)
{
	int	x;
	int	y;

	x = data->game.x;
	y = data->game.y;
	if (data->game.map[x][y - 1] != '1')
	{
		if (data->game.map[x][y - 1] == 'C')
		{
			data->game.collected++;
			data->game.map[x][y - 1] = '0';
		}
		exit_player_left(data, x, y);
		move_player(data, x, y - 1);
		data->game.y--;
		data->game.steps++;
		ft_printf("moved left --> steps : %d\n", data->game.steps);
	}
	return (0);
}

int	move(int key, t_data *data)
{
	if (data->game.coll == data->game.collected)
		return (move_end(key, data));
	if (key == UP)
		return (move_up(data));
	if (key == DOWN)
		return (move_down(data));
	if (key == LEFT)
		return (move_left(data));
	if (key == RIGHT)
		return (move_right(data));
	if (key == ESC)
		return (quit(data), 0);
	else
		return (0);
}
