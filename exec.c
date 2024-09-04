#include <minishell.h>

void exec_cmd(char **args, t_mini *mini)
{
	char *exec = find_exec(args[0], mini->path);
	int	ret;
	if(!exec)
	{
		printf("%s: command not found\n", args[0]);
		return ;
	}
	int pid = fork();
	if(pid > 0)
	{
		waitpid(pid, &ret, 0);
		//printf("ret = %d\n", ret);
		mini->status = (ret & 0xff00) >> 8;
	}
	else {
		execve(exec, args, NULL);
	}
}
