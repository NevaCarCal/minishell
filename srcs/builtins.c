/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:50:00 by antigravity       #+#    #+#             */
/*   Updated: 2025/11/26 02:50:25 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

void	builtin_echo(t_minishell *shell, char **args)
{
	int	i;
	int	n_flag;

	(void)shell;
	i = 1;
	n_flag = 0;
	while (args[i] && ft_strncmp(args[i], "-n", 3) == 0)
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', 1);
	shell->exit_code = 0;
}

void	builtin_cd(t_minishell *shell, char **args)
{
	int	i;

	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		shell->exit_code = 1;
		return ;
	}
	if (!args[1])
	{
		i = 0;
		while (shell->envp[i] && ft_strncmp(shell->envp[i], "HOME=", 5) != 0)
			i++;
		if (shell->envp[i])
		{
			if (chdir(shell->envp[i] + 5) != 0)
			{
				perror("cd");
				shell->exit_code = 1;
			}
			else
				shell->exit_code = 0;
		}
		else
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			shell->exit_code = 1;
		}
	}
	else
	{
		if (chdir(args[1]) != 0)
		{
			perror("cd");
			shell->exit_code = 1;
		}
		else
			shell->exit_code = 0;
	}
}

void	builtin_pwd(t_minishell *shell)
{
	char	cwd[4096];

	if (getcwd(cwd, 4096))
	{
		ft_putstr_fd(cwd, 1);
		ft_putchar_fd('\n', 1);
		shell->exit_code = 0;
	}
	else
	{
		perror("pwd");
		shell->exit_code = 1;
	}
}

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
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

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

static int	env_var_exists(char *var, char **envp)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (var[len] && var[len] != '=')
		len++;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, len) == 0 && (envp[i][len] == '=' || envp[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

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

void	builtin_export(t_minishell *shell, char **args)
{
	int	i;
	int	idx;

	if (!args[1])
	{
		builtin_env(shell);
		return ;
	}
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			shell->exit_code = 1;
		}
		else
		{
			idx = env_var_exists(args[i], shell->envp);
			if (idx >= 0)
			{
				if (ft_strchr(args[i], '='))
					shell->envp = update_env_var(shell->envp, idx, args[i]);
			}
			else
				shell->envp = add_env_var(shell->envp, args[i]);
			shell->exit_code = 0;
		}
		i++;
	}
}

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
	i = 0;
	j = 0;
	while (envp[i])
	{
		if (i != index)
		{
			new_env[j] = envp[i];
			j++;
		}
		else
			free(envp[i]);
		i++;
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
