/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epiacent <epiacent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 17:17:58 by apuddu            #+#    #+#             */
/*   Updated: 2024/09/04 19:29:22 by epiacent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void exec_cmd(char **args, t_mini *mini)
{
	int		ret;
	char	*exec;
	
	exec = find_exec(args[0], mini->path);
	if(!exec)
	{
		printf("%s: command not found\n", args[0]);
		//free(args[0]);
		return;
	}
	free(args[0]);
	args[0] = exec;
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
