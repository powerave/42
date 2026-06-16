/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_pl.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 14:10:28 by jbias             #+#    #+#             */
/*   Updated: 2025/02/11 13:59:37 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

void	exit_player_up(t_data *data, int x, int y)
{
	if (data->game.map[x - 1][y] == 'E')
		data->game.map[x][y] = 'T';
	else if (data->game.map[x][y] == 'T')
		data->game.map[x + 1][y] = 'E';
}

void	exit_player_down(t_data *data, int x, int y)
{
	if (data->game.map[x + 1][y] == 'E')
		data->game.map[x][y] = 'T';
	else if (data->game.map[x][y] == 'T')
		data->game.map[x - 1][y] = 'E';
}

void	exit_player_left(t_data *data, int x, int y)
{
	if (data->game.map[x][y - 1] == 'E')
		data->game.map[x][y] = 'T';
	else if (data->game.map[x][y] == 'T')
		data->game.map[x][y + 1] = 'E';
}

void	exit_player_right(t_data *data, int x, int y)
{
	if (data->game.map[x][y + 1] == 'E')
		data->game.map[x][y] = 'T';
	else if (data->game.map[x][y] == 'T')
		data->game.map[x][y - 1] = 'E';
}
