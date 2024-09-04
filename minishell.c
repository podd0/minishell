#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <minishell.h>
#include <unistd.h>
#include <libft.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

void	crtlc(int signal)
{
	printf("\n");
	(void)signal;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();

}


int main()
{
	char 	*input;
	t_mini	mini;

	mini.path = get_path();
	signal(SIGINT, crtlc);
	while ( 1 )
	{
		input = readline("$ ");
		if (input == NULL)
		{
			break;
		}
		add_history(input);
		char **args = ft_split(input, ' ');
		if (args[0] != NULL)
		{
			exec_cmd(args, &mini);
		}
		ft_split_free(args);
		free(input);
	}
	ft_split_free(mini.path);
	return 0;
}
