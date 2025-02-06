#include "pipex_bonus.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    extern char **__environ;
    int **fd_pipes;
    int fd_in_out[2];
    int i;

    fd_pipes = NULL;
    if (argc < 5)
    {
        return (1);
    }
    if (argc == 25)
    {
        /*
        if(fd_pipes_management(&fd_pipes, argv, fd_in_out)) //create the pipe and connect the in and out file
            return 1;
        first_command(fd_in_out, fd_pipes, argv[2], __environ);
        last_command(fd_in_out,fd_pipes,argv[argc - 2], __environ);
        */
        if(fd_multi_pipes_management( &fd_pipes, argv, fd_in_out, argc))
            return 1;
        first_command(fd_in_out, fd_pipes, argv[2], __environ);
        last_command(fd_in_out,fd_pipes,argv[argc - 2], __environ);
    }
    else
    {
        int nb_command = argc - 5; 

        if(fd_multi_pipes_management( &fd_pipes, argv, fd_in_out, argc))
            return 1;
        first_command(fd_in_out, fd_pipes, argv[2], __environ);
        wait(NULL);
        int i = 1;
        while(nb_command > 0)
        {
            middle_command(fd_in_out, fd_pipes, argv[i+2], __environ, i);
            nb_command--;
            i++;
        }
        last_command(fd_in_out,fd_pipes,argv[argc - 2], __environ);
    }
       if (argc == 5)
       {
            close_mono_fd(fd_in_out, fd_pipes[0]);
            free(fd_pipes[0]);
            free(fd_pipes);
            return (0);
       }
       if(argc > 5)
       {
                close_all_fd(fd_in_out, fd_pipes);

            i = 0;
            while (fd_pipes[i])
            {
                free(fd_pipes[i]);
                i++;
            }
            free(fd_pipes);
       }

    return 1;
}

char *get_env(char *name, char **env)
{
    char *buff;
    char *return_env;
    int i;
    int j;

    i = 0;
    while (env[i])
    {
        j = 0;
        while(env[i][j] && env[i][j] != '=')
        {
            j++;
        }
        buff = ft_substr(env[i], 0, j);
        if (!ft_strncmp(name, buff, ft_strlen(buff)))
            {
                free(buff);      
                return ft_substr(env[i], j + 1 ,ft_strlen(env[i]) - j);
            }  
        free(buff);      
        i++;
    }
    return NULL;
}

char *get_path(char *cmd, char **env)
{
    char **all_path;
    char *temp;
    int i;

    all_path = path_generator(cmd, env);
    temp = NULL;
    i = 0;
    while(all_path[i])
    {
        if (access(all_path[i], F_OK | X_OK) == 0)
        {
            temp = ft_strdup(all_path[i]);
            ft_free_tab(all_path);
            return temp;
        }
        i++;
    }
    ft_free_tab(all_path);
    write(2, "cmd not found", 13);
    return NULL;
}

void ft_free_tab(char **tab)
{
    int i;

    i = 0;
    if(tab)
    {
        while(tab[i])
        {
            free(tab[i]);
            i++;
        }
        free(tab);
    }
}

char **path_generator(char *cmd, char **env)
{
    char **all_path;
    char **cmds;
    char *temp;
    int i;
    
    i = 0;
    temp = get_env("PATH", env); 
    all_path = ft_split(temp, ':');
    free(temp);
    cmds = ft_split(cmd, ' ');
    while (all_path[i])
    {
        temp = ft_strjoin(all_path[i], "/");
        free(all_path[i]);
        all_path[i] = ft_strdup(temp);
        free(temp);
        temp = ft_strjoin(all_path[i], cmds[0]);
        free(all_path[i]);
        all_path[i] = ft_strdup(temp);
        free(temp);
       i++;
    }
    ft_free_tab(cmds);
    return all_path;
}

int fd_mono_managment(char **argv, int *fd_in_out, int *fd_pipes)
{
    if (access(argv[1], F_OK | R_OK) == -1)
	    return 1;
    else
	    fd_in_out[0] = open(argv[1], O_RDONLY);
	if(access(argv[4], F_OK | W_OK)== -1)
		fd_in_out[1] = open(argv[4], O_CREAT | O_RDWR, 0777);
	else
		fd_in_out[1] = open(argv[4], O_RDWR, 0777);
    if(pipe(fd_pipes)== -1)
        return 1;
    return 0;
}

int fd_multi_managment(char **argv, int *fd_in_out, int argc)
{
    if (access(argv[1], F_OK | R_OK) == -1)
	    return 1;
    else
	    fd_in_out[0] = open(argv[1], O_RDWR);

	if(access(argv[argc - 1], F_OK ) == -1)
		fd_in_out[1] = open(argv[argc-1], O_CREAT | O_RDWR, 0777);
	else
		fd_in_out[1] = open(argv[argc-1], O_RDWR, 0777);
    return 0;
}

void ft_free_pipes(int **tab)
{
    int i;

    i = 0;
    if(tab)
    {
        while(tab[i])
        {
            free(tab[i]);
            i++;
        }
        free(tab);
    }
}

int fd_pipes_management(int ***fd_pipes, char **argv, int *fd_in_out)
{
    *fd_pipes = malloc(sizeof(int *) * 2);
    if (!(*fd_pipes))
    {
       return (1); // Check malloc success
    }
    (*fd_pipes)[0] = malloc(2 * sizeof(int));
    if (!(*fd_pipes)[0]) 
    {
        free(*fd_pipes);
        return (1); // Check malloc success
    }
    (*fd_pipes)[1] = NULL;
    if (fd_mono_managment(argv, fd_in_out, (*fd_pipes)[0]))
    {
        free((*fd_pipes)[0]);
        free(*fd_pipes);
        return (1);
    }
    return 0;
    }  

    int fd_multi_pipes_management(int ***fd_pipes, char **argv, int *fd_in_out, int argc)
    {
        int nb_of_pipe;
        int i;

        nb_of_pipe = (argc-4);

        if (fd_multi_managment(argv, fd_in_out, argc))
            return 1;
        i = -1;
        *fd_pipes = malloc(sizeof(int *)  * (nb_of_pipe + 1));
        if (!(*fd_pipes))
            return (1);
        while(++i < nb_of_pipe)
        {
            (*fd_pipes)[i] = malloc(2 * sizeof(int));
            if (!(*fd_pipes)[i]) 
            {
                while (--i >= 0)
                    free((*fd_pipes)[i]);
                free(*fd_pipes);
                return (1); // Check malloc success
            }
            pipe((*fd_pipes)[i]);
        }
        (*fd_pipes)[i] = NULL;
        return 0;
    }

void first_command(int fd_in_out[2], int **fd_pipes, char *cmd, char **env)
{
    int pid;
    char *path;
    char **cmds;

    path = get_path(cmd, env);
    if(!path)
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

void last_command(int fd_in_out[2], int **fd_pipes, char *cmd, char **env)
{
    int pid;
    char *path;
    char **cmds;
    int i;

    i = 0;
    while (fd_pipes[i + 1] != NULL)
    {
        i++;
    }
    path = get_path(cmd, env);
    if(!path)
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

void	close_mono_fd(int fd_in_out[], int fd_pipes[])
{
	close(fd_in_out[0]);
	close (fd_in_out[1]);
	close(fd_pipes[0]);
	close(fd_pipes[1]);
}

void	close_all_fd(int fd_in_out[], int *fd_pipes[])
{
    int i;

    i = 0;
	close(fd_in_out[0]);
	close (fd_in_out[1]);
	
    while (fd_pipes[i])
    {
        close(fd_pipes[i][0]);
	    close(fd_pipes[i][1]);
        i++;
    }
}



void middle_command( int *fd_in_out, int **fd_pipes, char *cmd, char **env, int i)
{
    int pid;
    char *path;
    char **cmds;

    path = get_path(cmd, env);
    if(!path)
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