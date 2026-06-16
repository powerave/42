/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 11:20:38 by jbias             #+#    #+#             */
/*   Updated: 2024/12/06 15:16:18 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	ft_clear_buffer(char *rd_buffer, int rd_checker)
{
	ssize_t	i;

	i = 0;
	while (i < rd_checker)
	{
		rd_buffer[i] = '\0';
		i++;
	}
	return ;
}

char	*ft_line(char *full_buffer)
{
	char			*line;
	long int		i;

	i = 0;
	while (full_buffer[i] && full_buffer[i] != '\n')
		i++;
	if (full_buffer[i] == '\n')
		i++;
	line = ft_calloc((i + 1), sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (full_buffer[i] && full_buffer[i] != '\n')
	{
		line[i] = full_buffer[i];
		i++;
	}
	if (full_buffer[i] == '\n')
		line[i] = full_buffer[i];
	return (line);
}

char	*saved(char *full_buffer)
{
	char			*save;
	long int		i;
	long int		j;

	i = 0;
	j = 0;
	while (full_buffer[i] && full_buffer[i] != '\n')
		i++;
	if (full_buffer[i] == '\n')
		i++;
	if (ft_strlen(full_buffer) - i < 1)
		return (free(full_buffer), NULL);
	save = ft_calloc((ft_strlen(full_buffer) - i + 1), sizeof(char));
	if (!save)
		return (free(full_buffer), NULL);
	while (full_buffer[i])
		save[j++] = full_buffer[i++];
	free(full_buffer);
	return (save);
}

char	*read_file(int fd, char *full_buffer)
{
	char			*buffer;
	long int		bytes;

	buffer = ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!buffer)
		return (NULL);
	bytes = 1;
	while ((!ft_strchr(buffer, '\n') || !ft_strchr(buffer, '\0')) && bytes > 0)
	{
		ft_clear_buffer(buffer, bytes);
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes < 0)
			return (free(full_buffer), free(buffer), NULL);
		if (bytes == 0)
			return (free(buffer), full_buffer);
		if (!full_buffer)
		{
			full_buffer = ft_calloc(1, 1);
			if (!full_buffer)
				return (free(buffer), full_buffer);
		}
		full_buffer = ft_strjoin(full_buffer, buffer);
	}
	return (free(buffer), full_buffer);
}

char	*get_next_line(int fd)
{
	static char	*full_buffer = NULL;
	char		*line;

	if (fd < 0 && BUFFER_SIZE < 1 && read(fd, 0, 0))
		return (NULL);
	full_buffer = read_file(fd, full_buffer);
	if (!full_buffer)
		return (NULL);
	line = ft_line(full_buffer);
	full_buffer = saved(full_buffer);
	return (line);
}
