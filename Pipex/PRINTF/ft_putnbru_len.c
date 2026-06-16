/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbru_len.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:10:22 by jbias             #+#    #+#             */
/*   Updated: 2024/11/29 16:36:53 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void	ft_putcharu(char c)
{
	write(1, &c, 1);
}

int	ft_putnbru_len(unsigned int n)
{
	int				len;
	unsigned int	nb;

	len = 0;
	nb = n;
	if (nb >= 10)
	{
		len += ft_putnbru_len(nb / 10);
		len += ft_putnbru_len(nb % 10);
	}
	else
	{
		ft_putcharu((nb % 10) + '0');
		len++;
	}
	return (len);
}
/*
int	main(void)
{
	ft_putnbr_fd(-2147483648, 1);
	return (0);
}*/
