/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smarty <smarty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 21:45:54 by smarty            #+#    #+#             */
/*   Updated: 2023/12/10 22:55:38 by smarty           ###   ########.fr       */
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
void    create_pipe1(char **path, char **av, char **env, int ac, int y, t_cote fd[ac - 3])
{
	pid_t   childpid;

	childpid = fork();
	if (childpid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (childpid == 0)
	{
		close(fd[y].fd[1]);
		dup2(fd[y].fd[0], STDIN_FILENO);
		close(fd[y].fd[0]);
		ex_cmd2(path, av, env, y);
		exit(0);
	}
	else if (y == 0)
	{
		close(fd[y].fd[0]);
		dup2(fd[y].fd[0], STDIN_FILENO);
		close(fd[y].fd[1]);
		ex_cmd1(path, av, env, y);
		exit(0);
	}
}
void    create_pipen(char **path, char **av, char **env, int ac, int y, t_cote fd[ac - 3])
{
	pid_t   childpid;

	childpid = fork();
	if (childpid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (childpid == 0)
	{
		close(fd[y].fd[0]);
		dup2(fd[y - 1].fd[1], STDIN_FILENO);
		close(fd[y - 1].fd[1]);
		ex_cmd1(path, av, env, (y + 1));
		exit(0);
	}
}
void    create_last_pipe(char **path, char **av, char **env, int ac, int y, t_cote fd[ac - 3])
{
	pid_t   childpid;

	childpid = fork();
	if (childpid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (childpid == 0)
	{
		close(fd[y].fd[0]);
		dup2(fd[y].fd[1], STDOUT_FILENO);
		close(fd[y].fd[1]);
		ex_cmd1(path, av, env, (y + 1));
		exit(0);
	}
}
void    create_pipe(char **path, char **av, char **env, int ac, int y, t_cote fd[ac - 3])
{
	pid_t	childpid;
	
	childpid = fork();
	if (childpid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (childpid == 0)
	{
		dup2(fd[y].fd[1], STDOUT_FILENO);
		close(fd[y].fd[0]);
		close(fd[y].fd[1]);
		ex_cmd2(path, av, env, y);
		exit(0);
	}
	else
	{
		close(fd[y].fd[1]);
		dup2(fd[y].fd[0], STDIN_FILENO);
		close(fd[y].fd[0]);
		ex_cmd1(path, av, env, y);
		exit(0);
	}
}

void	create_childpid(char **path, int ac, char **av, char **env, int y, t_cote fd[ac - 3])
{
	int    i;

	i = 0;
	while (i < ac - 4)
	{
		if (pipe(fd[y].fd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	i = 0;
	while (i <= ac - 2)
	{
		if (ac == 5)
		{
			create_pipe(path, av, env, ac, i, fd);
			break;
		}
		if (i == 0)
			create_pipe1(path, av, env, ac, i, fd);
		if (i > 0 && i < (ac - 2))
			create_pipen(path, av, env, ac, i, fd);
		if (i == (ac - 2))
			create_last_pipe(path, av, env, ac, i, fd);
		i++;
	}
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
	t_cote		fd[ac - 3];
	create_childpid(path, ac, av, env, i, fd);
	return (0);
}