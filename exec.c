/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 17:17:58 by apuddu            #+#    #+#             */
/*   Updated: 2024/09/04 17:47:03 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void exec_cmd(char **args, t_mini *mini)
{
	int		ret;
	char	*exec;
	
	exec = find_exec(args[0], mini->path);
	free(args[0]);
	args[0] = exec;
	if(!exec)
	{
		printf("%s: command not found\n", args[0]);
		return;
	}
	int pid = fork();
	if(pid > 0)
	{
		waitpid(pid, &ret, 0);
		mini->status_last = (ret & 0xff00) >> 8;
	}
	else {
		execve(exec, args, NULL);
	}
		
}
