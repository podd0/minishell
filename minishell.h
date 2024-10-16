/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 19:59:55 by apuddu            #+#    #+#             */
/*   Updated: 2024/10/16 18:26:23 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <libft.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <vector.h>
# include <errno.h>

# define ARG 1
# define PIPE 2
# define IN 3
# define OUT 4
# define APPEND 5
# define DOCUMENT 6
# define END 0

typedef struct s_token
{
	struct s_token	*prev;
	struct s_token	*next;
	char			*value;
	int				type;
}					t_token;

typedef struct s_command
{
	char			**args;
	int				has_document;
	int				fd_out;
	int				fd_in;
	int				pid;
	int				*pipe_in;
	int				*pipe_out;
}					t_command;

typedef struct s_commands
{
	t_command		*arr;
	int				size;
}					t_commands;

typedef struct s_mini
{
	char			**path;
	int				status_last;
	t_vstr			*env;
	t_token			*tokens;
	t_commands		commands;
	t_vch			*pwd;
}					t_mini;

extern int			g_signal;

typedef void		(*t_builtin)(t_command *command, t_mini *mini);

char				**get_path(char **env);
char				*find_exec(char *cmd, char **path);
int					exec_cmd(char **args, t_mini *mini);

char				*subst_env(char *line, char **env, t_mini *mini);
void				tokenize(char *line, t_mini *mini);
void				free_tokens(t_token *token);

t_commands			to_command_array(t_token *tokens, t_mini *mini);
void				free_commands(t_commands commands);
void				exec_shell_line(t_commands commands, t_mini *mini);
void				clean_exit(t_mini *mini, t_commands commands, int status);
void				vch_cat(t_vch *res, char *str);

void				set_env(t_mini *mini, char **env);
char				*find_var(char *name, char **env);

void				echo(t_command *command, t_mini *mini);
void				mini_exit(t_command *command, t_mini *mini);
void				env(t_command *command, t_mini *mini);
void				export(t_command *command, t_mini *mini);
void				unset(t_command *command, t_mini *mini);
void				cd(t_command *command, t_mini *mini);
void				print_pwd(t_command *command, t_mini *mini);

int					is_variable_name(char c);

void				merge_sort(t_vstr *vec);
t_vch				*vch_from_string(char *s);
void				vch_set_string(t_vch *v, char *s);

void				replace_or_add_variable(char **splitted, t_mini *mini,
						char *var);
void				export_many_params(char **var, t_mini *mini);
char				*join_name_value(char *name, char *value);
char				**split_var(char *key_val);
int					find_var_index(char *name, t_vstr *env);
t_builtin			get_builtin(char *cmd);
int					exec_command(t_command *command, t_mini *mini);
void				pipe_builtin(t_builtin builtin, t_command *command,
						t_mini *mini);
void				free_tokens(t_token *token);
int					get_type(char **line);
char				*match_until(char **line, char *charset, int skip);
char				*skip_whitespace(char *line);

int					num_commands(t_token *tokens);
t_command			*init_commands(int count);
int					count_args(t_token *token);
t_token				*next_command(t_token *token);
t_token				*next_arg(t_token *token);

void				free_commands(t_commands commands);
void				sigquit(int signal);
void				pass(int signal);
void				crtlc(int signal);

#endif