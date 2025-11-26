/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 13:07:06 by ncarrera          #+#    #+#             */
/*   Updated: 2025/11/26 02:45:41 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	g_signal = 130;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

static void	process_line(char *line, t_minishell *shell)
{
	t_command	*cmds;

	if (*line)
	{
		add_history(line);
		cmds = parse_input(line, shell);
		if (cmds)
		{
			execute_command(shell, cmds);
			free_commands(cmds);
		}
	}
	free(line);
}

static char	**dup_envp(char **envp)
{
	char	**new_env;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

static int	init_shell(t_minishell *shell, char **envp, int argc)
{
	if (argc != 1)
	{
		printf("Error: no arguments allowed\n");
		return (1);
	}
	shell->exit_code = 0;
	shell->exit_loop = 0;
	shell->envp = dup_envp(envp);
	if (!shell->envp)
		return (1);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_minishell	shell;

	(void)argv;
	if (init_shell(&shell, envp, argc))
		return (1);
	setup_signals();
	while (1)
	{
		line = readline("\001\033[1;35m\002minishell"
				"\001\033[1;36m\002$ \001\033[0m\002");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		process_line(line, &shell);
		if (g_signal != 0)
		{
			shell.exit_code = g_signal;
			g_signal = 0;
		}
		if (shell.exit_loop)
			break ;
	}
	rl_clear_history();
	return (shell.exit_code);
}

void	exec_builtin(t_minishell *shell, char **args)
{
	if (ft_strncmp(args[0], "cd", 3) == 0)
		builtin_cd(shell, args);
	else if (ft_strncmp(args[0], "pwd", 4) == 0)
		builtin_pwd(shell);
	else if (ft_strncmp(args[0], "env", 4) == 0)
		builtin_env(shell);
	else if (ft_strncmp(args[0], "exit", 5) == 0)
		builtin_exit(shell, args);
	else if (ft_strncmp(args[0], "echo", 5) == 0)
		builtin_echo(shell, args);
	else if (ft_strncmp(args[0], "export", 7) == 0)
		builtin_export(shell, args);
	else if (ft_strncmp(args[0], "unset", 6) == 0)
		builtin_unset(shell, args);
}
