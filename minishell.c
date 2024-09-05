#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <minishell.h>
#include <unistd.h>
#include <libft.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

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

int main()
{
	char 		*input;
	t_mini		mini;
	t_commands	commands;

	mini.path = get_path();
	signal(SIGINT, crtlc);
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
		commands = to_command_array(cp);
		if(commands.size > 0)
		{
			exec_shell_line(commands, &mini);
			free_commands(commands);
		}
		free_tokens(cp);
		free(input);
	}
	ft_split_free(mini.path);
	return 0;
}
