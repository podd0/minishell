/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epiacent <epiacent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:27:16 by apuddu            #+#    #+#             */
/*   Updated: 2024/10/08 16:30:57 by epiacent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	crtlc(int signal)
{
	g_signal = SIGINT;
	printf("\n");
	(void)signal;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	pass(int signal)
{
	printf("\n");
	g_signal = signal;
	(void)signal;
}

void	sigquit(int signal)
{
	(void)signal;
	rl_on_new_line();
	rl_redisplay();
	printf("  \b\b");
}
