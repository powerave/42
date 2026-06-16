/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:44:48 by jbias             #+#    #+#             */
/*   Updated: 2025/02/11 13:54:30 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

int	closed(char **map)
{
	int	i;
	int	j;

	j = -1;
	i = 0;
	while (map[i][++j] != '\n')
		if (map[i][j] != '1')
			return (0);
	j = ft_strlen(map[0]);
	while (map[i + 1])
	{
		if (map[i][0] != '1' || map[i][j - 2] != '1' || map[i][j - 1] != '\n')
			return (0);
		i++;
	}
	j = -1;
	while (map[i][++j] != '\n')
		if (map[i][j] != '1')
			return (0);
	return (1);
}

int	normed(char **map)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j] != '\n')
		{
			if (map[i][j] != 'E' && map[i][j] != 'C'
					&& map[i][j] != 'P' && map[i][j] != '1'
					&& map[i][j] != '0')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	is_playable(t_game *game)
{
	int	i;
	int	j;

	i = -1;
	while (game->map[++i])
	{
		j = -1;
		while (game->map[i][++j])
		{
			if (game->map[i][j] == 'E')
				game->exit++;
			else if (game->map[i][j] == 'C')
				game->coll++;
			else if (game->map[i][j] == 'P')
			{
				game->player++;
				game->x = i;
				game->y = j;
			}
		}
	}
	if (game->player == 1 && game->exit == 1 && game->coll > 0)
		return (1);
	return (0);
}

int	valid_map(t_game *game)
{
	game->steps = 0;
	game->player = 0;
	game->coll = 0;
	game->exit = 0;
	game->x = 0;
	game->y = 0;
	game->collected = 0;
	if (!closed(game->map))
		return (free_tab(game->map), 0);
	if (!is_playable(game) || !normed(game->map))
		return (free_tab(game->map), 0);
	if (!has_solution(game))
		return (free_tab(game->map), 0);
	return (1);
}
