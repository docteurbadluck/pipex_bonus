
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "libft.h"
#include "pipex.h"
#include <sys/wait.h>

int main(int argc, char **argv)
{
	extern char **__environ;
	int fd_in_out[2];
	int fd_pipes[2];
	char **cmds;
	
	if(argc != 5)
		return 1;
	if (access(argv[1], F_OK | R_OK) == -1)
		return 1;
	if(access(argv[4], F_OK | W_OK)== -1)
		fd_in_out[1] = open(argv[4], O_CREAT | O_RDWR, 0777);
	else
		fd_in_out[1] = open(argv[4], O_RDWR, 0777);
	if (pipe(fd_pipes) == -1)
	{
		perror("pipe failed");
		return 1;
	}
	fd_in_out[0] = open(argv[1], O_RDONLY);
	child_first_command(fd_in_out, fd_pipes, argv[2]);
	wait(NULL);
	child_second_command(fd_in_out, fd_pipes, argv[3]);
	close_fd(fd_in_out, fd_pipes);
    return 0;
}

int ft_strcmp(char *str1, char *str2)
{
	if (ft_strlen(str1) != ft_strlen(str2))
		return 1;
	return ft_strncmp(str1, str2, ft_strlen(str1));
}

char *ft_getenv(char *name, char **env)
{
	int i;
	int j;
	char *sub_str;

	i = 0;
	while (*env)
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
		{
			j++;
		}
		sub_str = ft_substr(env[i], 0, j);

		if (ft_strcmp(name, sub_str) == 0)
		{
			free(sub_str);
			return (env[i] + j + 1);
		}
		free(sub_str);
		i++;
	}
	return NULL;
}

char *get_path(char *cmd, char **env)
{
	char **all_path;
	char **cmd_and_arg;
	char *path_part;
	char *full_path;
	int i;
	
	i = -1;
	all_path = ft_split(ft_getenv("PATH", env), ':');
	cmd_and_arg = ft_split(cmd, ' ');
	while(all_path[++i] != NULL)
	{
		path_part = ft_strjoin(all_path[i], "/");
		full_path = ft_strjoin(path_part, cmd_and_arg[0]);
		free(path_part);
		if (access(full_path,F_OK | X_OK) == 0)
		{
			ft_free_tab(cmd_and_arg);
			ft_free_tab(all_path);
			return (full_path);
		}
		free(full_path);
	}	
	ft_free_tab(all_path);
	ft_free_tab(cmd_and_arg);
	return(cmd);
}

void	close_fd(int fd_in_out[], int fd_pipes[])
{
	close(fd_in_out[0]);
	close (fd_in_out[1]);
	close(fd_pipes[0]);
	close(fd_pipes[1]);
}

void child_first_command(int fd_in_out[], int fd_pipes[], char *cmd )
{
	int pid;
	char **cmds;
	char *path;

	path = get_path(cmd, __environ);
	if(!ft_strcmp(path, cmd))
	{
		perror("command not found");
		return ;
	}
	cmds = ft_split(cmd, ' ');
	pid = fork();
	if (pid == 0)
	{
		dup2(fd_in_out[0], STDIN_FILENO);
		dup2(fd_pipes[1], STDOUT_FILENO);
		close_fd(fd_in_out, fd_pipes);
		execve(path, cmds, NULL);
	}
	free(path);
	ft_free_tab(cmds);
}

void child_second_command(int fd_in_out[], int fd_pipes[], char *cmd)
{
	int pid;
	char **cmds;
	char *path;

	path = get_path(cmd, __environ);
	if(!ft_strcmp(path, cmd))
	{
		perror("command not found");
		return ;
	}
	cmds = ft_split(cmd, ' ');
	pid = fork();
	if (pid == 0)
	{
		dup2(fd_in_out[1], STDOUT_FILENO);
		dup2(fd_pipes[0], STDIN_FILENO);
		close_fd(fd_in_out, fd_pipes);
		execve(path, cmds, NULL);
	}
	free(path);
	ft_free_tab(cmds);
}

void ft_free_tab(char **tab)
{
	int i = 0;
	
	if(tab)
	{
		while (tab[i])
		{
			free(tab[i]);
			i++;
		}
		free(tab);
	}
}