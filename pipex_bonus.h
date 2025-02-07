/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: docteurbadluck <docteurbadluck@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 12:18:09 by docteurbadl       #+#    #+#             */
/*   Updated: 2025/02/07 12:18:10 by docteurbadl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "libft.h"
#include <sys/wait.h>
#include <errno.h>

// ***env_tool.c***
char	*my_getenv(char *name, char **env);

char	*get_path(char *cmd, char **env);

char	**path_generator(char *cmd, char **env);

void	ft_free_tab(char **tab);

// ***env_tool.c***
int		fd_multi_managment(char **argv, int *fd_in_out, int nb_of_pipe);

int		fd_multi_pipes_management(int ***fd_pipes, char **argv,
			int *fd_in_out, int argc);

void	close_all_fd(int fd_in_out[], int *fd_pipes[]);

void	free_pipes(int **fd_pipes);

// ***pipex_bonus.c***
void	first_command(int fd_in_out[2], int **fd_pipes, char *cmd, char **env);

void	last_command(int fd_in_out[2], int **fd_pipes, char *cmd, char **env);

void	middle_command(int *fd_in_out, int **fd_pipes, char *cmd, int i);
