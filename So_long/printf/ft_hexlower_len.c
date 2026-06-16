/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexlower_len.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:28:58 by jbias             #+#    #+#             */
/*   Updated: 2024/11/29 16:31:16 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

int	ft_hexlower_len(unsigned long long n)
{
	int		len;
	char	*base;

	base = "0123456789abcdef";
	len = 0;
	if (n >= 16)
	{
		len += ft_hexlower_len(n / 16);
		len += ft_hexlower_len(n % 16);
	}
	else
	{
		write(1, &base[n], 1);
		len++;
	}
	return (len);
}
