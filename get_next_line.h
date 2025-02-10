/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: docteurbadluck <docteurbadluck@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 12:09:41 by tdeliot           #+#    #+#             */
/*   Updated: 2025/02/10 11:26:42 by docteurbadl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include "libft.h"
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

//get_next_line.c
char	*get_next_line(int fd);
char	*ft_read(char *static_str, int fd);
char	*ft_get_line(char *static_str);
char	*ft_update_leftover(char *static_str);

//get_next_line_utils.c
char	*ft_strjoin2(char *str1, char *str2);
char	*ft_strchr2(const char *s, int c);
int		ft_strlen2(const char *str);

#endif