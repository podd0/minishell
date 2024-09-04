#include <minishell.h>


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
		free(input);
		char **args = ft_split(input, ' ');
		
		// printf("%s", input);
		// printf("\n");
	}

	return 0;

}
