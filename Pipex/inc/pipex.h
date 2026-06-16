/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 19:15:52 by jbias             #+#    #+#             */
/*   Updated: 2025/03/07 19:15:54 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "ft_printf.h"
# include "libft.h"
# include "get_next_line.h"

void	child(char **av, char **envp, int *fd);
void	parent(char **av, char **envp, int *fd);
void	error(void);
void	xecute(char *av, char **envp);
void	execute(char *av, char **envp);
char	*get_path(char *cmd, char **envp);
char	**fix_split(char **str);
int		find_index(char *str, char c);
void	pipex_b(int ac, char **av, char **envp, int i);
int		ft_gnl(char **str);
char	*get_path_b(char *cmd, char **envp);
void	child_b(char *av, char **envp);
void	here_doc(char *limiter);
int		open_fd(char *av, int i);
pid_t	pid1_fork(int *fd, char **av, char **envp);
pid_t	pid2_fork(int *fd, char **av, char **envp);

#endif
