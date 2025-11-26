/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 01:55:00 by antigravity       #+#    #+#             */
/*   Updated: 2025/11/26 13:16:25 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir_type	get_redir_type(char *token)
{
	if (ft_strncmp(token, ">>", 3) == 0)
		return (REDIR_APPEND);
	if (ft_strncmp(token, "<<", 3) == 0)
		return (REDIR_HEREDOC);
	if (ft_strncmp(token, ">", 2) == 0)
		return (REDIR_OUT);
	return (REDIR_IN);
}

static int	handle_token(t_command **curr, char *token, t_parse_info *info)
{
	char			*next_tok;
	t_redir_type	type;

	if (ft_strncmp(token, "|", 2) == 0)
	{
		(*curr)->next = new_command();
		*curr = (*curr)->next;
	}
	else if (is_operator(token[0]))
	{
		type = get_redir_type(token);
		next_tok = get_next_token(info->line, info->i);
		if (!next_tok)
			return (0);
		add_redirection(*curr, type, next_tok, info->shell);
		free(next_tok);
	}
	else
		add_argument(*curr, token, info->shell);
	return (1);
}

static int	parse_loop(t_command *head, char *line, t_minishell *shell)
{
	t_command		*curr;
	char			*token;
	int				i;
	t_parse_info	info;

	curr = head;
	i = 0;
	info.line = line;
	info.i = &i;
	info.shell = shell;
	while (1)
	{
		token = get_next_token(line, &i);
		if (!token)
			break ;
		if (!handle_token(&curr, token, &info))
		{
			free(token);
			return (0);
		}
		free(token);
	}
	return (1);
}

t_command	*parse_input(char *line, t_minishell *shell)
{
	t_command	*head;

	head = new_command();
	if (!head)
		return (NULL);
	if (!parse_loop(head, line, shell))
	{
		free_commands(head);
		return (NULL);
	}
	return (head);
}
