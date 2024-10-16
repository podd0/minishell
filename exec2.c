/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 15:17:02 by apuddu            #+#    #+#             */
/*   Updated: 2024/10/16 20:33:10 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	exec_builtin(t_commands commands, t_mini *mini, int i)
{
	t_builtin	builtin;

	builtin = get_builtin(commands.arr[i].args[0]);
	if (!builtin)
		return (0);
	pipe_builtin(builtin, commands.arr + i, mini);
	return (1);
}

void	exec_program(t_commands commands, t_mini *mini, int i)
{
	int	status;

	commands.arr[i].pid = fork();
	if (commands.arr[i].pid < 0)
		clean_exit(mini, commands, 1);
	else if (commands.arr[i].pid == 0)
	{
		status = exec_command(commands.arr + i, mini);
		clean_exit(mini, commands, status);
	}
	if (commands.arr[i].fd_in != STDIN_FILENO)
		close(commands.arr[i].fd_in);
	if (commands.arr[i].fd_out != STDOUT_FILENO)
		close(commands.arr[i].fd_out);
	if (commands.arr[i].pipe_in)
		close(commands.arr[i].pipe_in[0]);
	if (commands.arr[i].pipe_out)
		close(commands.arr[i].pipe_out[1]);
}

void	wait_commands(t_commands commands, t_mini *mini)
{
	int	i;
	int	ret;

	i = 0;
	while (i < commands.size)
	{
		if (commands.arr[i].pid)
		{
			waitpid(commands.arr[i].pid, &ret, 0);
			mini->status_last = (ret & 0xff00) >> 8;
		}
		i++;
	}
}

void	exec_shell_line(t_commands commands, t_mini *mini)
{
	int	i;
	int	*fd;

	i = 0;
	while (i < commands.size)
	{
		if (i < commands.size - 1)
		{
			fd = malloc(sizeof(int) * 2);
			if (pipe(fd) == -1)
				clean_exit(mini, commands, 1);
			commands.arr[i].pipe_out = fd;
			commands.arr[i + 1].pipe_in = fd;
		}
		if (!exec_builtin(commands, mini, i))
			exec_program(commands, mini, i);
		i++;
	}
	wait_commands(commands, mini);
}
