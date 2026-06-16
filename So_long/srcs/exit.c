/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:04:06 by jbias             #+#    #+#             */
/*   Updated: 2025/02/11 14:01:55 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

void	victory(t_data *data)
{
	ft_printf("Victory\n");
	kill_sprites(&data->sprite, data, 0);
	free_tab(data->game.map);
	exit (0);
}

void	quit(t_data *data)
{
	ft_printf("You have left the game\n");
	kill_sprites(&data->sprite, data, 0);
	free_tab(data->game.map);
	exit (0);
}

void	error_msg(char *msg)
{
	write(2, msg, ft_strlen(msg));
	exit (0);
}
