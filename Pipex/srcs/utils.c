/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 10:20:34 by jbias             #+#    #+#             */
/*   Updated: 2025/03/07 10:20:37 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void	error(void)
{
	perror("Error");
	exit(EXIT_FAILURE);
}

int	find_index(char *str, char c)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

void	execute(char *av, char **envp)
{
	char	**cmd;
	char	*path;

	cmd = ft_split(av, ' ');
	if (cmd[0] == NULL)
	{
		free_tab(cmd);
		perror ("Error");
		exit (127);
	}
	path = get_path(cmd[0], envp);
	if (!path)
	{
		free_tab(cmd);
		perror("Error:\nWrong path");
		exit (127);
	}
	if (execve(path, cmd, envp) == -1)
	{
		free_tab(cmd);
		perror("Error\n");
		exit (127);
	}
}

char	*get_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
			return (free_tab(paths), path);
		free(path);
		i++;
	}
	return (free_tab(paths), NULL);
}
