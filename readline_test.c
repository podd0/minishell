#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <minishell.h>
#include <unistd.h>
#include <libft.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
	char 	*input;
	t_mini	mini;
	int ret;

	mini.path = get_path();
	while ( 1 )
	{
		input = readline("Enter text: ");
		if (input == NULL)
		{
			rl_clear_history();
			return 0;
		}
		add_history(input);
		// free(input);
		char **args = ft_split(input, ' ');
		char *exec = find_exec(args[0], mini.path);
		if(!exec)
		{
			printf("%s: command not found\n", args[0]);
			continue;
		}
		int pid = fork();
		if(pid > 0)
		{
			waitpid(pid, &ret, 0);
		}
		else {
			execve(exec, args, NULL);
		}
		// printf("%s", input);
		// printf("\n");
	}

	return 0;

}
