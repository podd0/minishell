/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 17:17:58 by apuddu            #+#    #+#             */
/*   Updated: 2024/10/16 20:31:02 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	exec_cmd(char **args, t_mini *mini)
{
	char	*exec;
	char	*old_exec;

	old_exec = args[0];
	exec = find_exec(args[0], mini->path);
	if (!exec)
	{
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return (127);
	}
	args[0] = exec;
	execve(exec, args, mini->env->arr);
	perror(old_exec);
	free(old_exec);
	if (errno == 2)
		return (127);
	if (errno == 13)
		return (126);
	return (1);
}

void	clean_exit(t_mini *mini, t_commands commands, int status)
{
	free_commands(commands);
	free_tokens(mini->tokens);
	rl_clear_history();
	vstr_map(mini->env, (void (*)(char *))free);
	vstr_free(mini->env);
	ft_split_free(mini->path);
	vch_free(mini->pwd);
	exit(status);
}

int	exec_command(t_command *command, t_mini *mini)
{
	if (command->pipe_in)
	{
		dup2(command->pipe_in[0], STDIN_FILENO);
	}
	if (command->pipe_out)
	{
		dup2(command->pipe_out[1], STDOUT_FILENO);
		close(command->pipe_out[0]);
	}
	dup2(command->fd_in, STDIN_FILENO);
	dup2(command->fd_out, STDOUT_FILENO);
	return (exec_cmd(command->args, mini));
}

t_builtin	get_builtin(char *cmd)
{
	int			i;
	static void	*builtins[7][2] = {{"exit", mini_exit}, {"echo", echo},
	{"export", export}, {"unset", unset}, {"cd", cd}, {"env", env},
	{"pwd", print_pwd}};

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
