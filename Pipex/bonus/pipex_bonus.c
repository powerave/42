/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 10:20:46 by jbias             #+#    #+#             */
/*   Updated: 2025/03/07 10:20:47 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void	xecute(char *av, char **envp)
{
	char	**cmd;
	char	*path;

	cmd = ft_split(av, ' ');
	if (cmd[0] == NULL)
	{
		free_tab(cmd);
		perror("Error");
		exit (127);
	}
	path = get_path_b(cmd[0], envp);
	if (!path)
	{
		free_tab(cmd);
		perror("Error");
		exit (127);
	}
	if (execve(path, cmd, envp) == -1)
	{
		free(path);
		free_tab(cmd);
		perror("Error");
		exit (127);
	}
}

static void	here_doc_child(int *fd, char *limiter)
{
	char	*line;

	line = '\0';
	close(fd[0]);
	while (1)
	{
		write(1, "> ", 2);
		if (ft_gnl(&line) == -1)
			break ;
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		free(line);
	}
	close(fd[1]);
	exit(EXIT_SUCCESS);
}

void	here_doc(char *limiter)
{
	pid_t	reader;
	int		fd[2];

	if (pipe(fd) == -1)
		error();
	reader = fork();
	if (reader == -1)
		error();
	if (reader == 0)
		here_doc_child(fd, limiter);
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		wait(NULL);
	}
}

void	pipex_b(int ac, char **av, char **envp, int i)
{
	int	fd_in;
	int	fd_out;

	if (ft_strncmp(av[1], "here_doc", 8) == 0)
	{
		if (ac < 6)
			error();
		i = 3;
		fd_out = open_fd(av[ac - 1], 0);
		here_doc(av[2]);
	}
	else
	{
		i = 2;
		fd_out = open_fd(av[ac - 1], 1);
		fd_in = open_fd(av[1], 2);
		dup2(fd_in, STDIN_FILENO);
	}
	while (i < ac - 2)
		child_b(av[i++], envp);
	dup2(fd_out, STDOUT_FILENO);
	xecute(av[ac - 2], envp);
}

int	main(int ac, char **av, char **envp)
{
	int	i;

	i = 0;
	if (ac > 4)
		pipex_b(ac, av, envp, i);
	else
	{
		write(2, "Error: Check arguments\n", 23);
		return (1);
	}
	return (0);
}
