/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_rem_exit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 13:25:49 by ncarrera          #+#    #+#             */
/*   Updated: 2025/11/26 13:25:55 by ncarrera         ###   ########.fr       */
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
