/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:30:00 by antigravity       #+#    #+#             */
/*   Updated: 2026/02/10 22:43:42 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*try_find_in_paths(char *cmd, char **paths)
{
	char		*path;
	int			i;
	struct stat	sb;

	i = 0;
	while (paths[i])
	{
		path = join_path(paths[i], cmd);
		if (path && access(path, X_OK) == 0)
		{
			if (stat(path, &sb) == 0 && !S_ISDIR(sb.st_mode))
			{
				ft_free_str_array(paths);
				return (path);
			}
		}
		free(path);
		i++;
	}
	ft_free_str_array(paths);
	return (NULL);
}

static char	**get_env_paths(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	return (ft_split(envp[i] + 5, ':'));
}

/* Check if a function is in the PATH. If not, retrieves path */
/* Function then searches for the exec and returns full path if found */
char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*abs_path;

	if (!cmd || !*cmd)
		return (NULL);
	abs_path = check_absolute_path(cmd);
	if (abs_path)
		return (abs_path);
	paths = get_env_paths(envp);
	if (!paths)
		return (NULL);
	return (try_find_in_paths(cmd, paths));
}
