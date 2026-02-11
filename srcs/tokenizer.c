/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 01:40:00 by antigravity       #+#    #+#             */
/*   Updated: 2026/02/11 20:17:49 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static int	get_operator_len(char *line)
{
	if (!line || !*line)
		return (0);
	if (line[0] == '>' && line[1] == '>')
		return (2);
	if (line[0] == '<' && line[1] == '<')
		return (2);
	return (1);
}

static int	get_word_len(char *line)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (line[i])
	{
		ft_update_quote(line[i], &quote);
		if (!quote && is_operator(line[i]))
			break ;
		if (!quote && ft_isspace(line[i]))
			break ;
		i++;
	}
	return (i);
}

char	*get_next_token(char *line, int *idx)
{
	int		start;
	int		len;
	char	*token;

	while (line[*idx] && ft_isspace(line[*idx]))
		(*idx)++;
	if (!line[*idx])
		return (NULL);
	start = *idx;
	if (is_operator(line[*idx]))
		len = get_operator_len(line + *idx);
	else
		len = get_word_len(line + *idx);
	token = malloc(sizeof(char) * (len + 1));
	if (!token)
		return (NULL);
	ft_strlcpy(token, line + start, len + 1);
	*idx += len;
	return (token);
}
