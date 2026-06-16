/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 10:20:52 by jbias             #+#    #+#             */
/*   Updated: 2025/03/07 10:20:54 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void	child_b(char *av, char **envp)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		error();
	pid = fork();
	if (pid == -1)
		error();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		xecute(av, envp);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		wait(NULL);
	}
}

int	open_fd(char *av, int i)
{
	int	fd;

	fd = 0;
	if (i == 0)
		fd = open(av, O_WRONLY | O_CREAT | O_APPEND, 0777);
	else if (i == 1)
		fd = open(av, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else if (i == 2)
	{
		fd = open(av, O_RDONLY, 0777);
		if (fd == -1)
			fd = open("/dev/null", O_RDONLY, 0777);
	}
	if (fd == -1)
		error();
	return (fd);
}

int	ft_gnl(char **str)
{
	char	*buffer;
	int		i;
	int		j;
	char	c;

	i = 0;
	j = 0;
	buffer = malloc(sizeof(char) * 4096);
	if (!buffer)
		return (-1);
	j = read(0, &c, 1);
	while (j && (c != '\n') && (c != '\0') && (i < 4093))
	{
		buffer[i++] = c;
		j = read(0, &c, 1);
	}
	if (i >= 4093 && (c != '\n' && c != '\0'))
		error ();
	if (j > 0 && c == '\n')
		buffer[i++] = '\n';
	buffer[i] = '\0';
	*str = buffer;
	if (j < 0)
		return (-1);
	return (0);
}

char	*get_path_b(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
		{
			free_tab(paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_tab(paths);
	return (NULL);
}
