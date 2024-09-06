/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 17:17:58 by apuddu            #+#    #+#             */
/*   Updated: 2024/09/06 14:39:19 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void exec_cmd(char **args, t_mini *mini)
{
	char	*exec;

	exec = find_exec(args[0], mini->path);
	if(!exec)
	{
		printf("%s: command not found\n", args[0]);
		return;
	}
	free(args[0]);
	args[0] = exec;
	execve(exec, args, NULL);
}
void	clean_exit(t_mini *mini, t_commands commands, int status)
{
	free_commands(commands);
	free_tokens(mini->tokens);
	exit(status);
}

void	exec_command(t_command *command, t_mini *mini)
{
	dup2(command->fd_in, STDIN_FILENO);
	dup2(command->fd_out, STDOUT_FILENO);
	exec_cmd(command->args, mini);
}

void	exec_commands(t_commands commands, t_mini *mini)
{
	int	i;
	int	pid;
	int	fd[2];

	i = 0;
	while (i < commands.size-1)
	{
		if (pipe(fd) == -1)
			clean_exit(mini, commands, 1);
		pid = fork();
		if (pid < 0)
			clean_exit(mini, commands, 1);
		else if (pid == 0)
		{
			dup2(fd[1], STDOUT_FILENO);
			close(fd[0]);
			exec_command(commands.arr + i, mini);
			clean_exit(mini, commands, 1);
		}
		dup2(fd[0], STDIN_FILENO);
		close(fd[1]);
		i++;
	}
	exec_command(commands.arr + i, mini);
	clean_exit(mini, commands, 1);
}

void	exec_shell_line(t_commands	commands, t_mini *mini)
{
	int	pid;
	int	ret;

	pid = fork();
	if (pid < 0)
		clean_exit(mini, commands, 1);
	if(pid == 0)
	{
		exec_commands(commands, mini);
	}
	else
	{
		g_proc_running = 1;
		waitpid(pid, &ret, 0);
		g_proc_running = 0;
		mini->status_last = (ret & 0xff00) >> 8;
	}

}
