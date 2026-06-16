/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 11:37:53 by jbias             #+#    #+#             */
/*   Updated: 2024/12/11 16:25:26 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stddef.h>
# include <string.h>

char	*get_next_line(int fd);
char	*ft_strjoin(char *s1, char *s2);
int		ft_strchr(char *str, char c);
int		ft_strlen(char	*str);
void	*ft_calloc(int nmemb, int size);

#endif
