#include <minishell.h>

int	g_proc_running = 0;

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

int main(int argc, char **argv, char **env)
{
	char 		*input;
	t_mini		mini;
	t_commands	commands;

	(void)argc;
	(void)argv;
	set_env(&mini, env);
	
	mini.path = get_path(env);
	signal(SIGINT, crtlc);
	signal(SIGQUIT, sigquit);
	while ( 1 )
	{
		input = readline("$ ");
		if (input == NULL)
		{
			printf("\n");
			break;
		}
		add_history(input);
		t_token *tokens = tokenize(input, &mini);
		t_token *cp = tokens;
		while(tokens)
		{
			printf("TYPE = %d, value = %s\n", tokens->type, tokens->value);
			tokens = tokens->next;
		}
		mini.tokens = cp;
		commands = to_command_array(cp, &mini);
		if(commands.size > 0)
		{
			exec_shell_line(commands, &mini);
			free_commands(commands);
		}
		free_tokens(cp);
		free(input);
	}
	ft_split_free(mini.path);
	vstr_map(mini.env, (void (*)(char *)) free);
	vstr_free(mini.env);
	return 0;
}
