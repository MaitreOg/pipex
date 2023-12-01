/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smarty <smarty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 19:37:15 by smarty            #+#    #+#             */
/*   Updated: 2023/11/30 14:55:25 by smarty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include <sys/types.h>
# include <fcntl.h>

char	*ft_strstr(char *str, char *to_find);
char	**ft_split(char *str, char c);
char	*ft_strjoin(char *s1, char *s2);
void	ft_putendl_fd(char *s, int fd);
int		ft_strlen(char *str);
void	free_cmd(char **cmd);
void	free_path(char **path);
char	**find_path(char **env);
#endif