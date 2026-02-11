/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_envs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 13:23:18 by ncarrera          #+#    #+#             */
/*   Updated: 2026/02/11 20:46:12 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Prints entire envp out to stdout */
void	builtin_env(t_minishell *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		if (ft_strchr(shell->envp[i], '='))
		{
			ft_putstr_fd(shell->envp[i], 1);
			ft_putchar_fd('\n', 1);
		}
		i++;
	}
	shell->exit_code = 0;
}

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/* If unhandled, things like 'exit 1 2' or 'exit abc' could segfault */
/* It could also lead to unexpected exits when the shell shouldn't close */
void	builtin_exit(t_minishell *shell, char **args)
{
	ft_putstr_fd("exit\n", 1);
	if (args[1])
	{
		if (!is_numeric(args[1]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit(2);
		}
		if (args[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			shell->exit_code = 1;
			return ;
		}
		exit(ft_atoi(args[1]));
	}
	exit(shell->exit_code);
}

int	env_var_exists(char *var, char **envp)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (var[len] && var[len] != '=')
		len++;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, len) == 0 && (
				envp[i][len] == '=' || envp[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}
