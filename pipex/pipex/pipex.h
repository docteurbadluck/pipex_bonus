
char *ft_getenv(char *name, char **env);

int ft_strcmp(char *str1, char *str2);

char *get_path(char *cmd, char **env);

void	close_fd(int fd__in_out[], int fd_pipes[]);

void child_first_command(int fd_in_out[], int fd_pipes[], char *cmd);

void child_second_command(int fd_in_out[], int fd_pipes[], char *cmd);

void ft_free_tab(char **tab);

