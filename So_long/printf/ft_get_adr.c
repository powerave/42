/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_adr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 09:02:38 by jbias             #+#    #+#             */
/*   Updated: 2024/11/29 16:34:25 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

int	ft_get_adr(void *a)
{
	unsigned long long	i;
	int					len;

	len = 0;
	if (!a)
	{
		write (1, "(nil)", 5);
		return (5);
	}
	len += ft_putchar('0');
	len += ft_putchar('x');
	i = (unsigned long long)a;
	len += ft_hexlower_len(i);
	return (len);
}
/*
int	main(void)
{
	int	k = 5;
	int	*l = &k;
	

	ft_get_adr(l);
	return (0);
}*/
