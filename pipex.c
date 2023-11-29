/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smarty <smarty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 19:35:51 by smarty            #+#    #+#             */
/*   Updated: 2023/11/29 17:45:36 by smarty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**find_path(char **av, char **env)
{
	char	*env_path;
	char	**path;
	int		i;

	i = 0;
	while (ft_strstr(env[i], "PATH=") == 0)
		i++;
	env_path = ft_strstr(env[i], "PATH=");
	path = ft_split(env_path, ':');
	return (path);
}

void	ex_cmd1(char **path, char **av, char **env)
{
	int		i;
	char	**cmd1;

	cmd1 = ft_split(av[2], ' ');
	i = 0;
	while (path[i])
	{
		path[i] = ft_strjoin(path[i], "/");
		path[i] = ft_strjoin(path[i], cmd1[0]);
		if (access(path[i], F_OK | X_OK) == 0)
		{
			//write(2, "cmd1\n", 5);	
			//ft_putendl_fd(path[i], 2);
			//ft_putendl_fd(cmd1[0], 2);
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

void	ex_cmd2(char **path, char **av, char **env)
{
	int		i;
	char	**cmd2;

	cmd2 = ft_split(av[3], ' ');
	i = 0;
	while (path[i])
	{
		path[i] = ft_strjoin(path[i], "/");
		path[i] = ft_strjoin(path[i], cmd2[0]);
		if (access(path[i], F_OK | X_OK) == 0)
		{
			//ft_putendl_fd(path[i], 2);
			//ft_putendl_fd(cmd2[0], 2);
			//write(2, "cmd2\n", 10 / 2);	
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

void	create_childpid(char **path, char **av, char **env)
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
		ex_cmd2(path, av, env);
		exit(0);
	}
	else
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		ex_cmd1(path, av, env);
		exit(0);
	}
}

int	main(int ac, char **av, char **env)
{
	char	**path;
	int		fdi;
	int		fdo;

	fdi = open(av[1], 0, 0666);
	fdo = open(av[4], O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (fdo == -1 || fdi == -1)
		perror("fd");
	dup2(fdi, STDIN_FILENO);
	dup2(fdo, STDOUT_FILENO);
	close(fdi);
	close(fdo);
	path = find_path(av, env);
	create_childpid(path, av, env);
	return (0);
}

void	free_cmd(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
}
