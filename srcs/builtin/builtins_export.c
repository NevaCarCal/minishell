/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 13:25:14 by ncarrera          #+#    #+#             */
/*   Updated: 2026/02/11 19:53:29 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**add_env_var(char **envp, char *var)
{
	int		i;
	char	**new_env;

	i = 0;
	while (envp[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (envp);
	i = 0;
	while (envp[i])
	{
		new_env[i] = envp[i];
		i++;
	}
	new_env[i] = ft_strdup(var);
	new_env[i + 1] = NULL;
	free(envp);
	return (new_env);
}

/* Allows to update any var with an ID while handling memory automatically */
static char	**update_env_var(char **envp, int idx, char *var)
{
	free(envp[idx]);
	envp[idx] = ft_strdup(var);
	return (envp);
}

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	export_arg(t_minishell *shell, char *arg)
{
	int	idx;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		shell->exit_code = 1;
	}
	else
	{
		idx = env_var_exists(arg, shell->envp);
		if (idx >= 0)
		{
			if (ft_strchr(arg, '='))
				shell->envp = update_env_var(shell->envp, idx, arg);
		}
		else
			shell->envp = add_env_var(shell->envp, arg);
		shell->exit_code = 0;
	}
}

void	builtin_export(t_minishell *shell, char **args)
{
	int	i;

	if (!args[1])
	{
		builtin_env(shell);
		return ;
	}
	i = 1;
	while (args[i])
	{
		export_arg(shell, args[i]);
		i++;
	}
}
