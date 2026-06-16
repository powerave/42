/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 16:42:27 by jbias             #+#    #+#             */
/*   Updated: 2024/12/03 15:01:21 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_operator(va_list args, char c)
{
	int	len;

	len = 0;
	if (c == 'c')
		len += ft_putchar(va_arg(args, int));
	else if (c == 's')
		len += ft_putstr(va_arg(args, char *));
	else if (c == 'p')
		len += ft_get_adr(va_arg(args, void *));
	else if (c == 'd' || c == 'i')
		len += ft_putnbr_len(va_arg(args, int));
	else if (c == 'u')
		len += ft_putnbru_len(va_arg(args, unsigned int));
	else if (c == 'x')
		len += ft_hexlower_len(va_arg(args, unsigned int));
	else if (c == 'X')
		len += ft_hexupper_len(va_arg(args, unsigned int));
	else if (c == '%')
		len += ft_putchar('%');
	return (len);
}

int	ft_printf(const char *str, ...)
{
	va_list		args;
	int			len;
	char		*op;
	int			i;

	if (!str)
		return (-1);
	i = 0;
	op = "cspdiuxX%";
	len = 0;
	va_start(args, str);
	while (str[i])
	{
		if ((str[i] == '%') && strchr(op, str[i + 1]))
		{
			len += ft_operator(args, str[i + 1]);
			i++;
		}
		else
			len += ft_putchar(str[i]);
		i++;
	}
	va_end(args);
	return (len);
}
/*
int	main(void)
{

	char	a;
	char	*b;
	void	*c;
	int	d;
	int	e;
	int	f;
	int	i;
	
	f = 4687;
	d = 10;
	e = 10;
	c = &a;
	b = "coucou hibou";
	a = '';
//	ft_printf("char : %c\n", a);
//	printf("char : %c\n", a);
//	ft_printf("str : %s\n", b);
//	printf("str : %s\n", b);
//	ft_printf("adr : %p\n", c);
//	printf("len pf : %d\n\n", printf("adr : %p\n", c));
//	ft_printf("int : %i, %d\n", d, e);
//	printf("len pf : %d\n\n", printf("int : %i, %d\n", d, e));
//	ft_printf("u int : %u\n", f);
//	printf("len pf : %d\n\n", printf("u int : %u\n", f));
	printf("%d\n", printf(0));
	printf("%d\n", ft_printf(0));
	return (0);
}*/
