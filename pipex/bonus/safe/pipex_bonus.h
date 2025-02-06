#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "libft.h"
#include <sys/wait.h>
#include <errno.h>

char *get_env(char *name, char **env);

char *get_path(char *cmd, char **env);


char **path_generator(char *cmd, char **env);

int fd_mono_managment(char **argv, int *fd_in_out, int *fd_pipes);

void ft_free_tab(char **tab);

void ft_free_pipes(int **tab);

int fd_multi_managment(char **argv, int *fd_in_out, int nb_of_pipe);

int fd_pipes_management(int ***fd_pipes, char **argv, int *fd_in_out);

int fd_multi_pipes_management(int ***fd_pipes, char **argv, int *fd_in_out, int argc);

void first_command(int fd_in_out[2], int **fd_pipes, char *cmd, char **env);

void last_command(int fd_in_out[2], int **fd_pipes, char *cmd, char **env);

void	close_mono_fd(int fd_in_out[], int fd_pipes[]);

void	close_all_fd(int fd_in_out[], int *fd_pipes[]);

void middle_command( int *fd_in_out, int **fd_pipes, char *cmd, char **env, int i);
