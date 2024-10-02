/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:58:30 by apuddu            #+#    #+#             */
/*   Updated: 2024/10/02 14:58:30 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <minishell.h>

int		g_proc_running = 0;

void	crtlc(int signal)
{
	printf("\n");
	if (!g_proc_running)
	{
		(void)signal;
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sigquit(int signal)
{
	(void)signal;
	if (!g_proc_running)
	{
		rl_on_new_line();
		rl_redisplay();
		printf("  \b\b");
	}
	else
		printf("\n");
}

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

int	main(int argc, char **argv, char **env)
{
	char		*input;
	t_mini		mini;
	t_commands	commands;
	char		*prompt;

	(void)argc;
	(void)argv;
	init_mini(&mini, env);
	signal(SIGINT, crtlc);
	signal(SIGQUIT, sigquit);
	while (1)
	{
		prompt = ft_strjoin(mini.pwd->arr, " $ ");
		input = readline(prompt);
		free(prompt);
		if (input == NULL)
		{
			printf("\n");
			break ;
		}
		mini.tokens = tokenize(input, &mini);
		commands = to_command_array(mini.tokens, &mini);
		mini.commands = commands;
		if (mini.tokens)
			add_history(input);
		if (commands.size > 0)
		{
			exec_shell_line(commands, &mini);
			free_commands(commands);
		}
		free_tokens(mini.tokens);
		free(input);
	}
	rl_clear_history();
	vstr_map(mini.env, (void (*)(char *))free);
	vstr_free(mini.env);
	vch_free(mini.pwd);
	ft_split_free(mini.path);
}
