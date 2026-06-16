/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 11:42:17 by jbias             #+#    #+#             */
/*   Updated: 2025/02/11 14:02:17 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

int	main(int ac, char **av)
{
	t_data	data;

	init_struct(&data, &data.sprite, &data.game);
	if (!parsing(ac, av, &data.game))
		return (1);
	hook_master(&data);
	return (0);
}
