/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smarty <smarty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:28:58 by smarty            #+#    #+#             */
/*   Updated: 2023/12/14 19:11:34 by smarty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*read_function(int fd,char *stash)
{
	int		newl;
	char	*buffer;

	buffer = malloc(BUFFER_SIZE + 1);
	newl = -1;
	while (newl == -1)
	{
		if (read(fd, buffer, BUFFER_SIZE) < 0)
			return (NULL);
		buffer[BUFFER_SIZE] = 0;
		stash = ft_strjoin(stash, buffer);
		newl = check_newl(stash);
	}
	return (stash);
}

char	*find_str(char *stash)
{
	int		i;
	int		len;
	char	*str;

	i = 0;
	len = ft_strlen_newl(stash);
	str = malloc(len + 1);
	while (stash[i] != '\n')
	{
		str[i] = stash[i];
		i++;
	}
	str[i] = 0;
	return (str);
}

char	*next_line(char *stash)
{
	int			i;
	int			y;
	char	*str;

	i = 0;
	y = 0;
	while (stash[i] != '\n' && stash[i])
		i++;
	str = (char *)malloc(ft_strlen(stash) - (i + 1));
	while (stash[i])
	{
		str[y] = stash[i];
		y++;
		i++;
	}
	str[y] = 0;
	return (str);
}

char	*get_next_line(int fd)
{
	static	char	*stash;
	char			*str;
	
	if (!stash)
	{
		stash = malloc(1);
		stash = "\0";
	}
	if (fd < 0)
		return (NULL);
	stash = read_function(fd, stash);
	str = find_str(stash);
	stash = next_line(stash);
	return (str);
}