/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:48:29 by jbias             #+#    #+#             */
/*   Updated: 2024/11/29 16:37:58 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdlib.h>
# include <stddef.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>

int		ft_printf(const char *str, ...);
char	*ft_strchr(const char *s, int c);
int		ft_putnbru_len(unsigned int n);
int		ft_putnbr_len(int n);
int		ft_hexlower_len(unsigned long long n);
int		ft_hexupper_len(unsigned int n);
int		ft_putchar(char c);
int		ft_putstr(const char *str);
int		ft_get_adr(void *a);
int		ft_operator(va_list args, char c);

#endif
