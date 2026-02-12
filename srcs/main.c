/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 13:07:06 by ncarrera          #+#    #+#             */
/*   Updated: 2026/02/12 15:53:40 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

static void	handle_sig_interrupt(int sig)
{
	g_signal = 128 + sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	setup_signals(void)
{
	signal(SIGINT, handle_sig_interrupt);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	shell;

	(void)argv;
	if (init_shell(&shell, envp, argc))
		return (1);
	setup_signals();
	loop_shell(&shell);
	rl_clear_history();
	free_env(shell.envp);
	return (shell.exit_code);
}
