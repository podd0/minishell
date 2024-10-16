/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:33:55 by apuddu            #+#    #+#             */
/*   Updated: 2024/10/15 20:39:17 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	here_document(t_command *command, char *separator)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
		return (1);
	command->has_document = 1;
	command->fd_in = fd[0];
	while (1)
	{
		line = readline("> ");
		if (!line)
			ft_putendl_fd("warning : here document closed by end of file",
				STDERR_FILENO);
		if (!line || ft_strncmp(line, separator, ft_strlen(separator) + 1) == 0)
			break ;
		ft_putendl_fd(line, fd[1]);
		free(line);
	}
	close(fd[1]);
	free(line);
	return (0);
}

int open_and_sub(char *filename, int *save_here, int flags)
{
	int	fd;

	if(flags == O_RDONLY)
		fd = open(filename, flags);
	else
		fd = open(filename, flags, 0666);
	if (fd < 0)
	{
		perror(filename);
		if(errno == 2)
			return (127);
		if(errno == 13)
			return (126);
		return (1);
	}
	*save_here = fd;
	return (0);	
}

int	check_type(t_command *command, t_token *tokens, int *i)
{
	if (tokens->type == ARG)
		command->args[(*i)++] = ft_strdup(tokens->value);
	else if (tokens->type == DOCUMENT)
	{
		if (here_document(command, tokens->value))
			return (2);
	}
	else if (tokens->type == IN && !command->has_document)
		return (open_and_sub(tokens->value, &command->fd_in, O_RDONLY));
	else if (tokens->type == OUT)
		return (open_and_sub(tokens->value, &command->fd_out, O_WRONLY | O_CREAT | O_TRUNC));
	else if (tokens->type == APPEND)
		return (open_and_sub(tokens->value, &command->fd_out, O_WRONLY | O_CREAT | O_APPEND));
	return (0);
}

int	make_single_command(t_command *command, t_token *tokens)
{
	int	argc;
	int	i;
	int	ret;

	i = 0;
	argc = count_args(tokens);
	command->args = ft_calloc(argc + 1, sizeof(char *));
	while (tokens && tokens->type != PIPE)
	{
		ret = check_type(command, tokens, &i);
		if (ret)
			return (ret);
		tokens = tokens->next;
	}
	return (0);
}

int	check_okay(t_commands commands)
{
	int	i;

	i = 0;
	while (i < commands.size)
	{
		if (commands.arr[i].args[0] == NULL)
		{
			ft_putendl_fd("Error: malformed command", 2);
			return (0);
		}
		i++;
	}
	return (1);
}

t_commands	checks(t_commands commands, t_mini *mini, int count, int status)
{
	if (count < 0 || !check_okay(commands))
	{
		free_commands(commands);
		mini->status_last = status;
		return ((t_commands){NULL, -1});
	}
	return (commands);
}

t_commands	to_command_array(t_token *tokens, t_mini *mini)
{
	t_command	*command_arr;
	int			count;
	int			i;
	t_commands	commands;
	int			ret;

	count = num_commands(tokens);
	command_arr = init_commands(count);
	i = 0;
	commands = (t_commands){command_arr, count};
	while (i < count)
	{
		ret = make_single_command(command_arr + i, tokens);
		if (ret == 2)
			clean_exit(mini, commands, 1);
		else if (ret != 0)
			return (checks(commands, mini, -1, ret));
		tokens = next_command(tokens);
		i++;
	}
	return (checks(commands, mini, count, 1));
}
