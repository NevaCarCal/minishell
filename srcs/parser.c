/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 01:55:00 by antigravity       #+#    #+#             */
/*   Updated: 2025/11/26 02:45:41 by ncarrera         ###   ########.fr       */
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

t_command	*parse_input(char *line, t_minishell *shell)
{
	t_command	*head;
	t_command	*curr;
	char		*token;
	int			i;

	head = new_command();
	if (!head)
		return (NULL);
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
			free_commands(head);
			return (NULL);
		}
		free(token);
	}
	return (head);
}
