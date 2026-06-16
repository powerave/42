/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexupper_len.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:42:11 by jbias             #+#    #+#             */
/*   Updated: 2024/11/29 16:30:03 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_hexupper_len(unsigned int n)
{
	int		len;
	char	*base;

	base = "0123456789ABCDEF";
	len = 0;
	if (n >= 16)
	{
		len += ft_hexupper_len(n / 16);
		len += ft_hexupper_len(n % 16);
	}
	else
	{
		len += ft_putchar(base[n % 16]);
	}
	return (len);
}
