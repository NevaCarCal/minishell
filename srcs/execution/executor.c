/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:15:00 by antigravity       #+#    #+#             */
/*   Updated: 2025/11/26 13:54:45 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_external(t_command *cmd, t_minishell *shell)
{
	char		*path;
	struct stat	sb;
	int			err;

	path = find_path(cmd->args[0], shell->envp);
	if (!path)
	{
		handle_exec_error(cmd->args[0], shell);
		exit(127);
	}
	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
		exit(126);
	}
	execve(path, cmd->args, shell->envp);
	err = errno;
	perror("execve");
	if (err == EACCES || err == EISDIR)
		exit(126);
	if (err == ENOENT)
		exit(127);
	exit(1);
}

void	exec_child(t_command *cmd,
	t_minishell *shell, int prev_fd, int *pip)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (pip)
	{
		close(pip[0]);
		dup2(pip[1], STDOUT_FILENO);
		close(pip[1]);
	}
	if (!handle_redirections(cmd->redirs))
		exit(1);
	if (!cmd->args || !cmd->args[0] || !*cmd->args[0])
		exit(0);
	if (is_builtin(cmd->args[0]))
	{
		exec_builtin(shell, cmd->args);
		exit(shell->exit_code);
	}
	exec_external(cmd, shell);
}

void	handle_parent(int *prev_fd, int *pip)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (pip)
	{
		close(pip[1]);
		*prev_fd = pip[0];
	}
	else
		*prev_fd = -1;
}

void	clean_empty_arg(t_command *cmd)
{
	int	i;

	if (cmd->args && cmd->args[0] && !*cmd->args[0] && cmd->args[1])
	{
		i = 0;
		while (cmd->args[i + 1])
		{
			cmd->args[i] = cmd->args[i + 1];
			i++;
		}
		cmd->args[i] = NULL;
	}
}

void	exec_single_builtin(t_command *cmd, t_minishell *shell)
{
	int	saved_std[2];

	saved_std[0] = dup(STDIN_FILENO);
	saved_std[1] = dup(STDOUT_FILENO);
	if (handle_redirections(cmd->redirs))
		exec_builtin(shell, cmd->args);
	else
		shell->exit_code = 1;
	dup2(saved_std[0], STDIN_FILENO);
	dup2(saved_std[1], STDOUT_FILENO);
	close(saved_std[0]);
	close(saved_std[1]);
}
