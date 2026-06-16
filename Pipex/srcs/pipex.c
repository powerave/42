/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 10:20:23 by jbias             #+#    #+#             */
/*   Updated: 2025/03/07 10:20:30 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void	child(char **av, char **envp, int *fd)
{
	int	fd_in;

	fd_in = open(av[1], O_RDONLY, 0777);
	if (fd_in == -1)
	{
		close(fd[0]);
		close(fd[1]);
		perror("Error");
		exit (1);
	}
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	dup2(fd_in, STDIN_FILENO);
	close(fd[1]);
	close(fd_in);
	execute(av[2], envp);
}

void	parent(char **av, char **envp, int *fd)
{
	int	fd_out;

	fd_out = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd_out == -1)
	{
		close(fd[1]);
		close(fd[0]);
		perror("Error");
		exit (1);
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	close(fd[0]);
	close(fd_out);
	execute(av[3], envp);
}

pid_t	pid1_fork(int *fd, char **av, char **envp)
{
	pid_t	pid;

	if (pipe(fd) == -1)
		error();
	pid = fork();
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		error();
	}
	if (pid == 0)
		child(av, envp, fd);
	return (pid);
}

pid_t	pid2_fork(int *fd, char **av, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		error();
	}
	if (pid == 0)
		parent(av, envp, fd);
	return (pid);
}

int	main(int ac, char **av, char **envp)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	pid1 = 0;
	pid2 = 0;
	if (ac == 5)
	{
		pid1 = pid1_fork(fd, av, envp);
		pid2 = pid2_fork(fd, av, envp);
		close(fd[0]);
		close(fd[1]);
		waitpid(pid1, &status, 0);
		waitpid(pid2, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			exit(WEXITSTATUS(status));
	}
	else
		return (write(2, "Error : check arguments\n", 25), 1);
	return (0);
}
