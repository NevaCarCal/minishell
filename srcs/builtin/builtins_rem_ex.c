/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_rem_ex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 13:25:49 by ncarrera          #+#    #+#             */
/*   Updated: 2025/11/26 13:58:22 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**remove_env_var(char **envp, int index)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	while (envp[i])
		i++;
	new_env = malloc(sizeof(char *) * i);
	if (!new_env)
		return (envp);
	i = -1;
	j = 0;
	while (envp[++i])
	{
		if (i != index)
			new_env[j++] = envp[i];
		else
			free(envp[i]);
	}
	new_env[j] = NULL;
	free(envp);
	return (new_env);
}

void	builtin_unset(t_minishell *shell, char **args)
{
	int	i;
	int	idx;

	i = 1;
	while (args[i])
	{
		idx = env_var_exists(args[i], shell->envp);
		if (idx >= 0)
			shell->envp = remove_env_var(shell->envp, idx);
		i++;
	}
	shell->exit_code = 0;
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
