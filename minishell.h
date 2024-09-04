/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 19:59:55 by apuddu            #+#    #+#             */
/*   Updated: 2024/09/04 18:47:44 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <libft.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

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
}	t_token;

typedef struct s_mini
{
	char	**path;
	int		status_last; 
	char	**env;
}	t_mini;

typedef struct s_command
{
	char	**args;
	int		fd_out;
	int		fd_in;
}	t_command;

char	**get_path(void);
char	*find_exec(char *cmd, char **path);
void	exec_cmd(char **args, t_mini *mini);


char	*subst_env(char* line, char **env);


#endif