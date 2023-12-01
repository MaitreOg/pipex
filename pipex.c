/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smarty <smarty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 19:35:51 by smarty            #+#    #+#             */
/*   Updated: 2023/12/01 18:56:29 by smarty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ex_cmd1(char **path, char **av, char **env, int y)
{
	int		i;
	char	**cmd1;

	cmd1 = ft_split(av[y + 2], ' ');
	i = 0;
	while (path[i])
	{
		path[i] = ft_strjoin(path[i], "/");
		path[i] = ft_strjoin(path[i], cmd1[0]);
		if (access(path[i], F_OK | X_OK) == 0)
		{
			if (execve(path[i], cmd1, env) == -1)
			{
				free_path(path);
				free_cmd(cmd1);
				perror("error cmd1");
			}
			break ;
		}
		free(path[i]);
		i++;
	}
}

void	ex_cmd2(char **path, char **av, char **env, int y)
{
	int		i;
	char	**cmd2;

	cmd2 = ft_split(av[y + 3], ' ');
	i = 0;
	while (path[i])
	{
		path[i] = ft_strjoin(path[i], "/");
		path[i] = ft_strjoin(path[i], cmd2[0]);
		if (access(path[i], F_OK | X_OK) == 0)
		{
			if (execve(path[i], cmd2, env) == -1)
			{
				free_path(path);
				free_cmd(cmd2);
				perror("error cmd2");
			}
			break ;
		}
		free(path[i]);
		i++;
	}
}

void	create_childpid2(char **path, char **av, char **env, int *fd, int y)
{
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	ex_cmd1(path, av, env, y);
	exit(0);
}

int	create_childpid(char **path, char **av, char **env, int y)
{
	int		fd[2];
	pid_t	childpid;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	childpid = fork();
	if (childpid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (childpid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		ex_cmd2(path, av, env, y);
		exit(0);
	}
	else
		create_childpid2(path, av, env, fd, y);
	y += 2;
	return (y);
}

int	main(int ac, char **av, char **env)
{
	char	**path;
	int		fdi;
	int		fdo;
	int		i;

	i = 0;
	fdi = open(av[1], 0, 0666);
	fdo = open(av[ac - 1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (fdo == -1 || fdi == -1)
		perror("fd");
	dup2(fdi, STDIN_FILENO);
	dup2(fdo, STDOUT_FILENO);
	close(fdi);
	close(fdo);
	path = find_path(env);
	if (ac == 5)
		create_childpid(path, av, env, i);
	else
	{
		while (i < (ac - 1))
			create_childpid(path, av, env, i);
	}
	return (0);
}
