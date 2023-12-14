/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smarty <smarty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 21:45:54 by smarty            #+#    #+#             */
/*   Updated: 2023/12/14 13:53:59 by smarty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ex_cmd(char **av, char **env, int y)
{
	int		i;
	char	**cmd;
	char	**path;

	cmd = ft_split(av[y], ' ');
	path = find_path(env);
	i = 0;
	while (path[i])
	{
		path[i] = ft_strjoin(path[i], "/");
		path[i] = ft_strjoin(path[i], cmd[0]);
		if (access(path[i], F_OK | X_OK) == 0)
		{
			if (execve(path[i], cmd, env) == -1)
			{
				free_path(path);
				free_cmd(cmd);
				perror("error cmd");
			}
			break ;
		}
		free(path[i]);
		i++;
	}
}
void    pipe_process(char **av, char **env, int i)
{
	pid_t   childpid;
	int		fd[2];
	
	if (pipe(fd) == -1)
		exit(EXIT_FAILURE);
	childpid = fork();
	if (childpid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (childpid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		ex_cmd(av, env, i);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		waitpid(childpid, NULL, 0);
	}
}
void create_file_doc(char **av, int fd[2])
{
	char	*line;

	close(fd[0]);
	while (1)
	{
		line = get_next_line(0);
		printf("%s\n", line);
		if (ft_strncmp(line, av[2], ft_strlen(av[2])) == 1)
		{
			free(line);
			exit(0);
		}
		write(fd[1], line, ft_strlen(line));
		free(line);
	}
}

void    here_doc(char **av)
{
	pid_t   childpid;
	int		fd[2];
	
	
	if (pipe(fd) == -1)
		exit(EXIT_FAILURE);
	childpid = fork();
	if (childpid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (childpid == 0)
		create_file_doc(av, fd);
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		waitpid(childpid, NULL, 0);
	}
}

/*int	main(int ac, char **av, char **env)
{
	int		i;
	int		fdi;
	int		fdo;
	
	fdi = open(av[1], 0, 0666);
	fdo = open(av[ac - 1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (fdo == -1 || fdi == -1)
		perror("fd");
	
	i = 2;
	dup2(fdi, STDIN_FILENO);
	while (i < ac -2)
	{
		pipe_process(av, env, i);
		i++;
	}
	dup2(fdo, STDOUT_FILENO);
	ex_cmd(av, env, i);
	return (0);
}*/
