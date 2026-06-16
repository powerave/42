/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   backtracking.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 21:55:53 by jbias             #+#    #+#             */
/*   Updated: 2025/02/11 14:03:39 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

char	**map_dup(char **map)
{
	char	**nmap;
	char	*line;
	int		i;

	i = 0;
	while (map[i])
		i++;
	nmap = (char **)malloc(sizeof(char *) * (i + 1));
	if (!nmap)
		return (NULL);
	i = 0;
	while (map[i])
	{
		line = ft_strdup(map[i]);
		if (!line)
			return (free_tab(nmap), NULL);
		nmap[i] = line;
		i++;
	}
	nmap[i] = 0;
	return (nmap);
}

void	backtracking(char **map, int x, int y)
{
	map[x][y] = 'A';
	if (map[x][y + 1] != '1' && map[x][y + 1] != 'A')
		backtracking(map, x, y + 1);
	if (map[x][y - 1] != '1' && map[x][y - 1] != 'A')
		backtracking(map, x, y - 1);
	if (map[x + 1][y] != '1' && map[x + 1][y] != 'A')
		backtracking(map, x + 1, y);
	if (map[x - 1][y] != '1' && map[x - 1][y] != 'A')
		backtracking(map, x - 1, y);
}

int	check_nmap(char **nmap)
{
	int	i;
	int	j;

	i = 0;
	while (nmap[i])
	{
		j = 0;
		while (nmap[i][j])
		{
			if (nmap[i][j] == 'C' || nmap[i][j] == 'E')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	has_solution(t_game *game)
{
	char	**nmap;

	nmap = map_dup(game->map);
	if (!nmap)
		return (0);
	backtracking(nmap, game->x, game->y);
	if (!check_nmap(nmap))
		return (free_tab(nmap), 0);
	free_tab(nmap);
	return (1);
}
