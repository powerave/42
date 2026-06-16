/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:59:52 by jbias             #+#    #+#             */
/*   Updated: 2025/02/11 13:43:23 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "get_next_line.h"
# include "ft_printf.h"
# include "../minilibx-linux/mlx.h"
# include "libft.h"
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <stddef.h>

# define MAX_HEIGHT 1000
# define MAX_WIDTH 1900
# define CLOSE_BUTTON 17

# define UP 119
# define DOWN 115
# define LEFT 97
# define RIGHT 100
# define ESC 65307

# define COLLECT "xpm/collect.xpm"
# define WALL "xpm/wall.xpm"
# define PLAYER "xpm/player.xpm"
# define FLOOR "xpm/floor.xpm"
# define EXIT "xpm/exit.xpm"
# define EXITPL "xpm/exit_pl.xpm"
# define XPM 100

typedef struct s_sprite
{
	void	*wall;
	void	*collect;
	void	*player;
	void	*floor;
	void	*exit;
	void	*exit_pl;
}	t_sprite;

typedef struct s_game
{
	char	**map;
	int		width;
	int		height;
	int		collected;
	int		steps;
	int		exit;
	int		coll;
	int		player;
	int		x;
	int		y;
}	t_game;

typedef struct s_data
{
	t_sprite	sprite;
	t_game		game;
	void		*mlx;
	void		*wind;
}	t_data;

void	kill_sprites(t_sprite *sprite, t_data *data, char *msg);
void	error_msg(char *msg);
void	display_map(t_data *data, t_sprite *sprite, t_game *game);
void	init_struct(t_data *data, t_sprite *sprite, t_game *game);
void	*sprite_load(void *s, char *file, t_sprite *sprite, t_data *data);
void	load_sprites(t_sprite *sprite, t_data *data);
void	hook_master(t_data *data);
char	**map_dup(char **map);
void	backtracking(char **map, int x, int y);
int		check_nmap(char **nmap);
int		has_solution(t_game *game);
void	victory(t_data *data);
void	quit(t_data *data);
void	put_xpm(t_data *data, void *s, int x, int y);
void	move_player(t_data *data, int y, int x);
int		move_end(int key, t_data *data);
int		valid_file(char *file);
int		line_count(char *file);
char	**create_map(char *file, t_game *game);
int		check_map(char **map);
int		parsing(int ac, char **av, t_game *game);
int		closed(char **map);
int		normed(char **map);
int		is_playable(t_game *game);
int		valid_map(t_game *game);
int		move(int key, t_data *data);
void	exit_player_up(t_data *data, int x, int y);
void	exit_player_down(t_data *data, int x, int y);
void	exit_player_right(t_data *data, int x, int y);
void	exit_player_left(t_data *data, int x, int y);

#endif
