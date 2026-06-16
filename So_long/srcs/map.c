/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 14:16:29 by jbias             #+#    #+#             */
/*   Updated: 2025/02/11 13:55:49 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

void	put_xpm(t_data *data, void *s, int x, int y)
{
	int	i;

	i = mlx_put_image_to_window(data->mlx, data->wind, s, x, y);
	if (i == -1)
		kill_sprites(&data->sprite, data, "Error\nCouldn't put xpm\n");
}

void	move_player(t_data *data, int y, int x)
{
	put_xpm(data, data->sprite.floor, data->game.y * XPM, data->game.x * XPM);
	put_xpm(data, data->sprite.player, x * XPM, y * XPM);
	if (data->game.map[y][x] == 'E')
	{
		if (data->game.coll != data->game.collected)
			put_xpm(data, data->sprite.exit_pl, x * XPM, y * XPM);
		else
			put_xpm(data, data->sprite.exit, data->game.x * XPM,
				data->game.y * XPM);
	}
	if (data->game.map[data->game.x][data->game.y] == 'E')
		put_xpm(data, data->sprite.exit,
			data->game.y * XPM, data->game.x * XPM);
}

void	display_map(t_data *data, t_sprite *sprite, t_game *game)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (game->map[y])
	{
		x = 0;
		while (game->map[y][x])
		{
			put_xpm(data, sprite->floor, x * XPM, y * XPM);
			if (game->map[y][x] == 'P')
				put_xpm(data, sprite->player, x * XPM, y * XPM);
			if (game->map[y][x] == '1')
				put_xpm(data, sprite->wall, x * XPM, y * XPM);
			if (game->map[y][x] == 'C')
				put_xpm(data, sprite->collect, x * XPM, y * XPM);
			if (game->map[y][x] == 'E')
				put_xpm(data, sprite->exit, x * XPM, y * XPM);
			if (game->map[y][x] == 'T')
				put_xpm(data, sprite->exit_pl, x * XPM, y * XPM);
			x++;
		}
		y++;
	}
}
