/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: docteurbadluck <docteurbadluck@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 12:09:09 by tdeliot           #+#    #+#             */
/*   Updated: 2025/02/10 12:58:39 by docteurbadl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

//return the next line of a file
//Use the static char * st_left_over two time,  first of all to read 
//if necessary and then to get the line to return
char	*get_next_line(int fd)
{
	static char	*st_left_over;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	st_left_over = ft_read(st_left_over, fd);
	if (!st_left_over)
		return (NULL);
	line = ft_get_line(st_left_over);
	st_left_over = ft_update_leftover(st_left_over);
	if (!*line)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

// this function "grow" the static_str until the next line or EOF
//  ft_strjoin(), free the "old"  static str to avoid leaks. 
char	*ft_read(char *static_str, int fd)
{
	char	*buff;
	int		read_byte;

	read_byte = 1;
	buff = malloc(BUFFER_SIZE + 1);
	if (!buff)
		return (NULL);
	while (!ft_strchr2(static_str, '\n') && read_byte != 0)
	{
		read_byte = read(fd, buff, BUFFER_SIZE);
		if (read_byte == -1)
		{
			free(buff);
			free(static_str);
			return (NULL);
		}
		buff[read_byte] = '\0';
		static_str = ft_strjoin2(static_str, buff);
	}
	free(buff);
	return (static_str);
}

//copy a line from static_str to line
//allocate + 2 to include '\n'
char	*ft_get_line(char *static_str)
{
	int		i;
	char	*line;

	i = 0;
	while (static_str[i] && static_str[i] != '\n')
		i++;
	line = malloc(i + 2);
	i = 0;
	while (static_str[i] && static_str[i] != '\n')
	{
		line[i] = static_str[i];
		i++;
	}
	if (static_str[i] == '\n')
	{
		line[i] = static_str[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

//copy whatever after the EOL free the current left_over
//and return the new one. if nothing to copy, return NULL
char	*ft_update_leftover(char *static_str)
{
	int		i;
	int		c;
	char	*new_leftover;

	i = 0;
	c = 0;
	while (static_str[c] && static_str[c] != '\n')
		c++;
	if (!static_str[c] || !static_str[++c])
	{
		free(static_str);
		return (NULL);
	}
	new_leftover = malloc (ft_strlen2(static_str) - c + 1);
	if (!new_leftover)
		return (NULL);
	while (static_str[++c])
	{
		new_leftover[i] = static_str[c];
		i++;
	}
	new_leftover[i] = '\0';
	free (static_str);
	return (new_leftover);
}

/*
int main()
{

	int fd = 0;
	char *result = get_next_line(fd);

	while(result)
	{
		printf("%s", result);
		free(result);
		result = get_next_line(fd);
	}
	free(result);
	return 0;
}
*/