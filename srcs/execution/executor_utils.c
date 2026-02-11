/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 01:00:00 by ncarrera          #+#    #+#             */
/*   Updated: 2026/02/11 19:43:49 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_absolute_path(char *cmd)
{
	if (ft_strncmp(cmd, "/", 1) == 0 || ft_strncmp(cmd, "./", 2) == 0)
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	return (NULL);
}

static void	handle_slash_error(char *cmd)
{
	struct stat	sb;

	if (stat(cmd, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		exit(126);
	}
	if (access(cmd, F_OK) == 0 && access(cmd, X_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		exit(126);
	}
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	exit(127);
}

void	handle_exec_error(char *cmd, t_minishell *shell)
{
	(void)shell;
	if (ft_strchr(cmd, '/'))
		handle_slash_error(cmd);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	exit(127);
}

char	*join_path(char *dir, char *cmd)
{
	char	*part_path;
	int		len1;
	int		len2;
	int		k;
	int		m;

	len1 = ft_strlen(dir);
	len2 = ft_strlen(cmd);
	part_path = malloc(len1 + len2 + 2);
	if (!part_path)
		return (NULL);
	k = 0;
	m = 0;
	while (m < len1)
		part_path[k++] = dir[m++];
	part_path[k++] = '/';
	m = 0;
	while (m < len2)
		part_path[k++] = cmd[m++];
	part_path[k] = '\0';
	return (part_path);
}
