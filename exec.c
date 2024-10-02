/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 17:17:58 by apuddu            #+#    #+#             */
/*   Updated: 2024/09/24 17:52:35 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	exec_cmd(char **args, t_mini *mini)
{
	char	*exec;

	exec = find_exec(args[0], mini->path);
	if (!exec)
	{
		printf("%s: command not found\n", args[0]);
		return ;
	}
	free(args[0]);
	args[0] = exec;
	execve(exec, args, mini->env->arr);
}

void	clean_exit(t_mini *mini, t_commands commands, int status)
{
	free_commands(commands);
	free_tokens(mini->tokens);
	rl_clear_history();
	vstr_map(mini->env, (void (*)(char *))free);
	vstr_free(mini->env);
	ft_split_free(mini->path);
	exit(status);
}

void	exec_command(t_command *command, t_mini *mini)
{
	if (command->pipe_in)
	{
		dup2(command->pipe_in[0], STDIN_FILENO);
		close(command->pipe_in[1]);
	}
	if (command->pipe_out)
	{
		dup2(command->pipe_out[1], STDOUT_FILENO);
		close(command->pipe_out[0]);
	}
	dup2(command->fd_in, STDIN_FILENO);
	dup2(command->fd_out, STDOUT_FILENO);
	exec_cmd(command->args, mini);
}

t_builtin	get_builtin(char *cmd)
{
	static void	*builtins[7][2] = {{"exit", mini_exit}, {"echo", echo},
			{"export", export}, {"unset", unset}, {"cd", cd}, {"env", env},
			{"pwd", print_pwd}};
	int			i;

	i = 0;
	while (i < 7)
	{
		if (ft_strncmp((char *)builtins[i][0], cmd,
				ft_strlen(builtins[i][0])) == 0)
			return ((t_builtin)builtins[i][1]);
		i++;
	}
	return (NULL);
}

void	pipe_builtin(t_builtin builtin, t_command *command, t_mini *mini)
{
	if (command->fd_in == STDIN_FILENO && command->pipe_in)
		command->fd_in = command->pipe_in[0];
	if (command->fd_out == STDOUT_FILENO && command->pipe_out)
		command->fd_out = command->pipe_out[1];
	builtin(command, mini);
	if (command->fd_in != STDIN_FILENO)
		close(command->fd_in);
	if (command->fd_out != STDOUT_FILENO)
		close(command->fd_out);
	if (command->pipe_in && command->fd_in != command->pipe_in[0])
		close(command->pipe_in[0]);
	if (command->pipe_out && command->fd_out != command->pipe_out[1])
		close(command->pipe_out[1]);
}

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
	commands.arr[i].pid = fork();
	if (commands.arr[i].pid < 0)
		clean_exit(mini, commands, 1);
	else if (commands.arr[i].pid == 0)
	{
		exec_command(commands.arr + i, mini);
		clean_exit(mini, commands, 1);
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

void	exec_commands(t_commands commands, t_mini *mini)
{
	int	i;
	int	*fd;
	int	ret;

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
	g_proc_running = 1;
	exec_commands(commands, mini);
	g_proc_running = 0;
}
