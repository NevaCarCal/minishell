/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_dirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:50:00 by antigravity       #+#    #+#             */
/*   Updated: 2025/12/12 16:30:00 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Check and return true if passed cmd is a cmd builtin function */
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

/* Searches for and changes the working directory of minishell to $HOME */
static void	cd_home(t_minishell *shell)
{
	int	i;

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

void	builtin_cd(t_minishell *shell, char **args)
{
	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		shell->exit_code = 1;
		return ;
	}
	if (!args[1])
		cd_home(shell);
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
