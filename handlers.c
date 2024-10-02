/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuddu <apuddu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:27:16 by apuddu            #+#    #+#             */
/*   Updated: 2024/10/02 16:32:18 by apuddu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	crtlc(int signal)
{
	printf("\n");
	if (1)
	{
		(void)signal;
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	pass(int signal)
{
	printf("\n");
	(void)signal;
}

void	sigquit(int signal)
{
	(void)signal;
	if (1)
	{
		rl_on_new_line();
		rl_redisplay();
		printf("  \b\b");
	}
}
