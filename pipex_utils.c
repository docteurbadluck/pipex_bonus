/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: docteurbadluck <docteurbadluck@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 12:18:22 by docteurbadl       #+#    #+#             */
/*   Updated: 2025/02/10 13:59:10 by docteurbadl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

// write the input in a temporary document.
//it also append the result in the outfile instead of rewrite it.
void	fd_here_doc_managment(char **argv, int *fd_in_out, int argc,
		int fd_temp)
{
	char	*result;
	char	*limiter;

	result = NULL;
	limiter = ft_strjoin(argv[2], "\n");
	while (1)
	{
		if (result != NULL)
			free(result);
		result = get_next_line(0);
		if (ft_strncmp(result, limiter, ft_strlen(limiter) + 1) != 0)
			write (fd_temp, result, ft_strlen(result));
		else
			break ;
	}
	free(result);
	free(limiter);
	fd_in_out[0] = open("temp.txt", O_RDWR);
	fd_in_out[1] = open(argv[argc - 1], O_APPEND | O_WRONLY | O_CREAT, 0777);
}

// fd_multi_managment handles the management of input and output fd.
//	It performs the following actions:
//	1. If the infile is valid and accessible.
//	2. If the outfile is valid, it opens it for reading and writing. 
//		If the outfile is invalid, it creates a new file with r/w. 
//	3. If the outfile already exists, it refresh it.
//	return (0) in case of success.
int	fd_multi_managment(char **argv, int *fd_in_out, int argc)
{
	int	fd_temp;

	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		if (access("temp.txt", F_OK) == -1)
			fd_temp = open("temp.txt", O_CREAT | O_RDWR, 0777);
		else
		{
			unlink("temp.txt");
			fd_temp = open("temp.txt", O_CREAT | O_RDWR, 0777);
		}
		fd_here_doc_managment(argv, fd_in_out, argc, fd_temp);
		return (0);
	}
	if (access(argv[1], F_OK | R_OK) == -1)
		return (-1);
	fd_in_out[0] = open(argv[1], O_RDWR);
	if (access(argv[argc - 1], F_OK) == -1)
		fd_in_out[1] = open(argv[argc - 1], O_CREAT | O_RDWR, 0777);
	else
	{
		unlink(argv[argc - 1]);
		fd_in_out[1] = open(argv[argc - 1], O_CREAT | O_RDWR, 0777);
	}
	return (0);
}

//	fd_multi_pipes_management allocates the necessary pipes.
//	based on the number of arguments (argc). It also manages I/O fd.
//	by calling fd_multi_managment. return 0 in case of success.
int	fd_multi_pipes_management(int ***fd_pipes, char **argv, int *fd_io, int ac)
{
	int	nb_of_pipe;
	int	i;

	nb_of_pipe = (ac - 4);
	if (fd_multi_managment(argv, fd_io, ac))
		return (-1);
	i = -1;
	*fd_pipes = malloc(sizeof(int *) * (nb_of_pipe + 1));
	if (!(*fd_pipes))
		return (-1);
	while (++i < nb_of_pipe)
	{
		(*fd_pipes)[i] = malloc(2 * sizeof(int));
		if (!(*fd_pipes)[i])
		{
			while (--i >= 0)
				free((*fd_pipes)[i]);
			free(*fd_pipes);
			return (-1);
		}
		pipe((*fd_pipes)[i]);
	}
	(*fd_pipes)[i] = NULL;
	return (0);
}

void	close_all_fd(int fd_in_out[], int *fd_pipes[])
{
	int	i;

	i = 0;
	close(fd_in_out[0]);
	close(fd_in_out[1]);
	while (fd_pipes[i])
	{
		close(fd_pipes[i][0]);
		close(fd_pipes[i][1]);
		i++;
	}
}

void	free_pipes(int **fd_pipes)
{
	int	i;

	i = 0;
	while (fd_pipes[i])
	{
		free(fd_pipes[i]);
		i++;
	}
	free(fd_pipes);
	if (!access("temp.txt", F_OK))
		unlink("temp.txt");
}
