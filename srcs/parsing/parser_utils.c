/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 01:50:00 by antigravity       #+#    #+#             */
/*   Updated: 2026/02/12 17:14:09 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_update_quote(char c, char *state)
{
	if (c == '\'' || c == '\"')
	{
		if (*state == 0)
			*state = c;
		else if (*state == c)
			*state = 0;
	}
}

static char	*process_char(char *arg, int *i, char quote, t_minishell *shell)
{
	char	*val;

	if (arg[*i] == '$' && quote == 0 && (arg[*i + 1] == '\''
			|| arg[*i + 1] == '\"'))
	{
		(*i)++;
		return (ft_strdup(""));
	}
	if (arg[*i] == '$' && quote != '\'' && (arg[*i + 1] == '?'
			|| ft_isalnum(arg[*i + 1]) || arg[*i + 1] == '_'))
		return (get_expansion_val(arg, i, shell));
	val = malloc(2);
	val[0] = arg[*i];
	val[1] = '\0';
	(*i)++;
	return (val);
}

char	*expand_variables(char *arg, t_minishell *shell)
{
	char	*new;
	char	*val;
	int		i;
	char	quote;

	new = ft_strdup("");
	if (!new)
		return (NULL);
	i = 0;
	quote = 0;
	while (arg[i])
	{
		ft_update_quote(arg[i], &quote);
		val = process_char(arg, &i, quote, shell);
		if (!val)
		{
			free(new);
			return (NULL);
		}
		new = append_val(new, val);
		if (!new)
			return (NULL);
	}
	return (new);
}

static char	**resize_args(char **args, char *new_arg)
{
	int		i;
	char	**new_args;

	i = 0;
	while (args && args[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (NULL);
	i = 0;
	while (args && args[i])
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[i] = new_arg;
	new_args[i + 1] = NULL;
	if (args)
		free(args);
	return (new_args);
}

int	add_argument(t_command *cmd, char *arg, t_minishell *shell)
{
	char	*expanded;
	char	*clean_arg;

	expanded = expand_variables(arg, shell);
	if (!expanded)
		return (0);
	if (expanded[0] == '\0')
	{
		free(expanded);
		return (1);
	}
	clean_arg = remove_quotes(expanded);
	free(expanded);
	if (!clean_arg)
		return (0);
	cmd->args = resize_args(cmd->args, clean_arg);
	if (!cmd->args)
	{
		free(clean_arg);
		return (0);
	}
	return (1);
}
