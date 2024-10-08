/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epiacent <epiacent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:58:30 by apuddu            #+#    #+#             */
/*   Updated: 2024/10/08 17:09:07 by epiacent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <minishell.h>

int		g_signal;

void	init_mini(t_mini *mini, char **env)
{
	char	*pwd;

	set_env(mini, env);
	mini->path = get_path(env);
	pwd = find_var("PWD", env);
	mini->status_last = 0;
	mini->pwd = vch_uninit(64);
	while (1)
	{
		pwd = getcwd(mini->pwd->arr, (size_t)mini->pwd->size);
		if (pwd == NULL && errno == ERANGE)
			vch_resize(mini->pwd, mini->pwd->size * 2);
		else if (pwd == NULL)
		{
			perror("pwd");
			exit(1);
		}
		else
		{
			break ;
		}
	}
}

int	main_func(t_mini *mini, char *input)
{
	if (input == NULL)
	{
		printf("\n");
		return (0);
	}
	mini->tokens = tokenize(input, mini);
	if (!mini->tokens)
		return (1);
	
	add_history(input);
	mini->commands = to_command_array(mini->tokens, mini);
	if (mini->commands.size > 0)
	{
		exec_shell_line(mini->commands, mini);
		free_commands(mini->commands);
	}
	free_tokens(mini->tokens);
	free(input);
	return (1);
}
void	set_status_if_signal(t_mini *mini)
{
	if (g_signal == SIGINT)
		mini->status_last = 130;
	else if (g_signal == SIGQUIT)
		mini->status_last = 131;
	g_signal = 0;
}

int	main(int argc, char **argv, char **env)
{
	char				*prompt;
	t_mini				mini;
	char				*input;

	(void)argc;
	(void)argv;
	init_mini(&mini, env);
	while (1)
	{
		signal(SIGINT, crtlc);
		signal(SIGQUIT, sigquit);
		prompt = ft_strjoin(mini.pwd->arr, " $ ");
		input = readline(prompt);
		set_status_if_signal(&mini);
		free(prompt);
		signal(SIGINT, pass);
		signal(SIGQUIT, pass);
		if (!main_func(&mini, input))
			break ;
	}
	rl_clear_history();
	vstr_map(mini.env, (void (*)(char *))free);
	vstr_free(mini.env);
	vch_free(mini.pwd);
	ft_split_free(mini.path);
}
