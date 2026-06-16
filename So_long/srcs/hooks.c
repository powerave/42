/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 10:08:13 by jbias             #+#    #+#             */
/*   Updated: 2025/02/11 13:57:37 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

void	init_struct(t_data *data, t_sprite *sprite, t_game *game)
{
	data->mlx = NULL;
	data->wind = NULL;
	sprite->exit = NULL;
	sprite->floor = NULL;
	sprite->player = NULL;
	sprite->wall = NULL;
	sprite->collect = NULL;
	sprite->exit_pl = NULL;
	game->map = NULL;
}

void	kill_sprites(t_sprite *sprite, t_data *data, char *msg)
{
	if (data->mlx && sprite->wall)
		mlx_destroy_image(data->mlx, sprite->wall);
	if (data->mlx && sprite->collect)
		mlx_destroy_image(data->mlx, sprite->collect);
	if (data->mlx && sprite->player)
		mlx_destroy_image(data->mlx, sprite->player);
	if (data->mlx && sprite->floor)
		mlx_destroy_image(data->mlx, sprite->floor);
	if (data->mlx && sprite->exit)
		mlx_destroy_image(data->mlx, sprite->exit);
	if (data->mlx && sprite->exit_pl)
		mlx_destroy_image(data->mlx, sprite->exit_pl);
	if (data->mlx)
	{
		mlx_clear_window(data->mlx, data->wind);
		mlx_destroy_window(data->mlx, data->wind);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
	}
	if (msg)
		return (error_msg(msg));
}

void	*sprite_load(void *s, char *file, t_sprite *sprite, t_data *data)
{
	int	width;
	int	height;

	s = mlx_xpm_file_to_image(data->mlx, file, &width, &height);
	if (!s || (width != height && width != XPM))
	{
		kill_sprites(sprite, data, "Error\nCouldn't load sprite\n");
		return (NULL);
	}
	return (s);
}

void	load_sprites(t_sprite *sprite, t_data *data)
{
	sprite->collect = sprite_load(sprite->collect, COLLECT, sprite, data);
	sprite->player = sprite_load(sprite->player, PLAYER, sprite, data);
	sprite->floor = sprite_load(sprite->floor, FLOOR, sprite, data);
	sprite->wall = sprite_load(sprite->wall, WALL, sprite, data);
	sprite->exit = sprite_load(sprite->exit, EXIT, sprite, data);
	sprite->exit_pl = sprite_load(sprite->exit_pl, EXITPL, sprite, data);
}

void	hook_master(t_data *data)
{
	int	f_width;
	int	f_height;

	f_width = data->game.width * XPM;
	f_height = data->game.height * XPM;
	if (f_height > MAX_HEIGHT || f_width > MAX_WIDTH)
		kill_sprites(&data->sprite, data, "Error\nOversized map.\n");
	data->mlx = mlx_init();
	if (!data->mlx)
		kill_sprites(&data->sprite, data, "Error\nCoundn't init.\n");
	data->wind = mlx_new_window(data->mlx, f_width, f_height, "so_long");
	if (!data->wind)
		kill_sprites(&data->sprite, data, "Error\nFailed to create window.\n");
	load_sprites(&data->sprite, data);
	display_map(data, &data->sprite, &data->game);
	mlx_key_hook(data->wind, &move, data);
	mlx_hook(data->wind, CLOSE_BUTTON, 0, (void *)quit, data);
	mlx_loop(data->mlx);
}
