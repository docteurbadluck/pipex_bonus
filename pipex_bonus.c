/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: docteurbadluck <docteurbadluck@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 12:18:00 by docteurbadl       #+#    #+#             */
/*   Updated: 2025/02/07 12:18:03 by docteurbadl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include <stdio.h>

//	pipex mimics the behavior of the pipe operator. 
//	extern char **environ is used to access 
//	the environment variables on POSIX systems.
int	main(int argc, char **argv)
{
	extern char	**environ;
	int			**fd_pipes;
	int			fd_io[2];
	int			i;
	int			nb_command;

	fd_pipes = NULL;
	i = 1;
	nb_command = argc - 5;
	if ((argc < 5) || fd_multi_pipes_management(&fd_pipes, argv, fd_io, argc))
	{
		ft_printf("Usage : <infile.txt> <cmd1> <cmd2> ... <outfile name>\n");
		return (-1);
	}
	first_command(fd_io, fd_pipes, argv[2], environ);
	while (nb_command > 0)
	{
		middle_command(fd_io, fd_pipes, argv[i + 2], i);
		nb_command--;
		i++;
	}
	last_command(fd_io, fd_pipes, argv[argc - 2], environ);
	close_all_fd(fd_io, fd_pipes);
	free_pipes(fd_pipes);
	return (0);
}

//	first_command sets up the first command in the pipeline. 
//	It takes input from the infile and outputs to the write 
//	end of the first pipe. It forks a child process to execute 
//	the command and handles the necessary redirections (STDIN, STDOUT).
void	first_command(int fd_in_out[2], int **fd_pipes, char *cmd, char **env)
{
	int		pid;
	char	*path;
	char	**cmds;

	path = get_path(cmd, env);
	if (!path)
		return ;
	cmds = ft_split(cmd, ' ');
	pid = fork();
	if (pid == 0)
	{
		dup2(fd_in_out[0], STDIN_FILENO);
		dup2(fd_pipes[0][1], STDOUT_FILENO);
		close_all_fd(fd_in_out, fd_pipes);
		if (execve(path, cmds, NULL) == -1)
			return ;
	}
	free(path);
	ft_free_tab(cmds);
	return ;
}

//	middle_command sets up the middle command in a pipeline. 
//	It handles input and output redirections for commands 
//	that are neither the first nor the last in the chain. 
//	The input is taken from the previous pipe's read end, 
//	and the output is sent to the current pipe's write end.
void	middle_command(int *fd_in_out, int **fd_pipes, char *cmd, int i)
{
	int			pid;
	char		*path;
	char		**cmds;
	extern char	**environ;

	path = get_path(cmd, environ);
	if (!path)
		return ;
	cmds = ft_split(cmd, ' ');
	pid = fork();
	if (pid == 0)
	{
		dup2(fd_pipes[i][1], STDOUT_FILENO);
		dup2(fd_pipes[i - 1][0], STDIN_FILENO);
		close_all_fd(fd_in_out, fd_pipes);
		if (execve(path, cmds, NULL) == -1)
			return ;
	}
	free(path);
	ft_free_tab(cmds);
	return ;
}

//	last_command sets up the last command in the pipeline. 
//	It handles input and output redirections for the final 
//	command in the chain. The input is taken from the previous 
//	pipe's read end, and the output is set to the write end 
//	of the output file descriptor (fd_in_out[1]).
void	last_command(int fd_in_out[2], int **fd_pipes, char *cmd, char **env)
{
	int		pid;
	char	*path;
	char	**cmds;
	int		i;

	i = 0;
	while (fd_pipes[i + 1] != NULL)
		i++;
	path = get_path(cmd, env);
	if (!path)
		return ;
	cmds = ft_split(cmd, ' ');
	pid = fork();
	if (pid == 0)
	{
		dup2(fd_in_out[1], STDOUT_FILENO);
		dup2(fd_pipes[i][0], STDIN_FILENO);
		close_all_fd(fd_in_out, fd_pipes);
		if (execve(path, cmds, NULL) == -1)
			return ;
	}
	free(path);
	ft_free_tab(cmds);
	return ;
}
