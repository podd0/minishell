/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 19:59:55 by apuddu            #+#    #+#             */
/*   Updated: 2024/09/06 18:26:28 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <libft.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <vector.h>

# define ARG 1
# define PIPE 2
# define IN 3
# define OUT 4
# define APPEND 5
# define DOCUMENT 6
# define END 0

extern int	g_proc_running;


typedef struct s_token
{
	struct s_token	*prev;
	struct s_token	*next;
	char			*value;
	int				type;
}	t_token;

typedef struct s_mini
{
	char	**path;
	int		status_last; 
	t_vstr	*env;
	t_token	*tokens;
}	t_mini;

typedef struct s_command
{
	char	**args;
	int		has_document;
	int		fd_out;
	int		fd_in;
}	t_command;

typedef struct s_commands
{
	t_command	*arr;
	int			size;
}	t_commands;

char	**get_path(char **env);
char	*find_exec(char *cmd, char **path);
void	exec_cmd(char **args, t_mini *mini);


char	*subst_env(char* line, char **env, t_mini *mini);
t_token	*tokenize(char *line, t_mini *mini);
void	free_tokens(t_token *token);


t_commands   to_command_array(t_token *tokens, t_mini *mini);
void		free_commands(t_commands commands);
void		exec_shell_line(t_commands	commands, t_mini *mini);
void	clean_exit(t_mini *mini, t_commands commands, int status);

void	set_env(t_mini *mini, char **env);

#endif