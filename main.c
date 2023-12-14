/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smarty <smarty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 03:39:18 by smarty            #+#    #+#             */
/*   Updated: 2023/12/14 13:41:12 by smarty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int ac, char **av, char **env)
{
	int		i;
	int		fdi;
	int		fdo;
	
    i = 3;
    
    
	fdo = open(av[ac - 1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
	//if (fdo == -1 || fdi == -1)
	//	perror("fd");
    if (ac == 5)
    {
        fdi = open(av[1], 0, 0666);
        dup2(fdo, STDOUT_FILENO);
        dup2(fdi, STDIN_FILENO);
        create_childpid(av, env, 2);
        return (0);
    }
    if (ft_strncmp("here_doc", av[1], 8) == 1)
        here_doc(av);
    else
    {
        i = 2;
        fdi = open(av[1], 0, 0666);
        dup2(fdi, STDIN_FILENO);
    }
	while (i < ac -2)
		pipe_process(av, env, i++);
	dup2(fdo, STDOUT_FILENO);
	ex_cmd(av, env, i);
    
    return (0);
}
