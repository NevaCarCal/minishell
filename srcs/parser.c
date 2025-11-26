/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 01:55:00 by antigravity       #+#    #+#             */
/*   Updated: 2025/11/26 13:01:45 by ncarrera         ###   ########.fr       */
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

static int	handle_token(t_command **curr, char *token, char *line, int *i, t_minishell *shell)
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
		next_tok = get_next_token(line, i);
		if (!next_tok)
			return (0);
		add_redirection(*curr, type, next_tok, shell);
		free(next_tok);
	}
	else
		add_argument(*curr, token, shell);
	return (1);
}

static int	parse_loop(t_command *head, char *line, t_minishell *shell)
{
	t_command	*curr;
	char		*token;
	int			i;

	curr = head;
	i = 0;
	while (1)
	{
		token = get_next_token(line, &i);
		if (!token)
			break ;
		if (!handle_token(&curr, token, line, &i, shell))
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
