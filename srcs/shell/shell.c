/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 13:59:37 by ncarrera          #+#    #+#             */
/*   Updated: 2026/02/11 14:56:59 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	free_env(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

void	process_line(char *line, t_minishell *shell)
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

int	init_shell(t_minishell *shell, char **envp, int argc)
{
	if (argc != 1)
	{
		printf("Error: no arguments allowed\n");
		return (1);
	}
	shell->exit_code = 0;
	shell->envp = dup_envp(envp);
	if (!shell->envp)
		return (1);
	return (0);
}

void	loop_shell(t_minishell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("\001\033[1;35m\002minishell"
			"\001\033[1;36m\002$ \001\033[0m\002");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		process_line(line, shell);
		if (g_signal != 0)
		{
			shell->exit_code = g_signal;
			g_signal = 0;
		}
	}
}
