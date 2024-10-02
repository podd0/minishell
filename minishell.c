/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:58:30 by apuddu            #+#    #+#             */
/*   Updated: 2024/10/02 16:31:57 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <minishell.h>

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
	mini->commands = to_command_array(mini->tokens, mini);
	if (mini->tokens)
		add_history(input);
	if (mini->commands.size > 0)
	{
		exec_shell_line(mini->commands, mini);
		free_commands(mini->commands);
	}
	free_tokens(mini->tokens);
	free(input);
	return (1);
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
