/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 13:41:17 by ncarrera          #+#    #+#             */
/*   Updated: 2026/02/11 12:17:08 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_redirection(t_command *cmd, t_redir *new)
{
	t_redir	*last;

	if (!cmd->redirs)
		cmd->redirs = new;
	else
	{
		last = cmd->redirs;
		while (last->next)
			last = last->next;
		last->next = new;
	}
}

int	add_redirection(t_command *cmd, t_redir_type type,
		char *file, t_minishell *shell)
{
	t_redir	*new;
	char	*expanded;
	char	*clean_file;

	expanded = expand_variables(file, shell);
	if (!expanded)
		return (0);
	clean_file = remove_quotes(expanded);
	free(expanded);
	if (!clean_file)
		return (0);
	new = new_redir(type, clean_file);
	free(clean_file);
	if (!new)
		return (0);
	append_redirection(cmd, new);
	return (1);
}
