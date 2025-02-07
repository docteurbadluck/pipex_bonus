/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: docteurbadluck <docteurbadluck@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 12:18:16 by docteurbadl       #+#    #+#             */
/*   Updated: 2025/02/07 12:18:17 by docteurbadl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

//  get_path searches for a valid path to the specified command in 
//  the directories listed in the PATH environment variable. 
//  It returns the first valid path where the command exists and is executable.
//  If no valid path is found, it prints an error and returns NULL.
char	*get_path(char *cmd, char **env)
{
	char	**all_path;
	char	*temp;
	int		i;

	all_path = path_generator(cmd, env);
	temp = NULL;
	i = 0;
	while (all_path[i])
	{
		if (access(all_path[i], F_OK | X_OK) == 0)
		{
			temp = ft_strdup(all_path[i]);
			ft_free_tab(all_path);
			return (temp);
		}
		i++;
	}
	ft_free_tab(all_path);
	write(2, "cmd not found", 13);
	return (NULL);
}

//	path_generator creates an array of full paths by combining each directory 
//	in the PATH environment variable with the given command. It returns an 
//	array of potential command paths where the executable could be located.
char	**path_generator(char *cmd, char **env)
{
	char	**all_path;
	char	**cmds;
	char	*temp;
	int		i;

	i = 0;
	temp = my_getenv("PATH", env);
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
	return (all_path);
}

//	This function is used to retrieve the value of the 
//  "NAME" environment variable.
//	We use it in this code to get "PATH",
//	which contains a colon-separated list of 
//  directories to search for executables.
char	*my_getenv(char *name, char **env)
{
	char	*buff;
	int		i;
	int		j;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
		{
			j++;
		}
		buff = ft_substr(env[i], 0, j);
		if (!ft_strncmp(name, buff, ft_strlen(buff)))
		{
			free(buff);
			return (ft_substr(env[i], j + 1, ft_strlen(env[i]) - j));
		}
		free(buff);
		i++;
	}
	return (NULL);
}

void	ft_free_tab(char **tab)
{
	int		i;

	i = 0;
	if (tab)
	{
		while (tab[i])
		{
			free(tab[i]);
			i++;
		}
		free(tab);
	}
}
