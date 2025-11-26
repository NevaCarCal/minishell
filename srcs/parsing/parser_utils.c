/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 01:50:00 by antigravity       #+#    #+#             */
/*   Updated: 2025/11/26 13:42:09 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*process_char(char *arg, int *i, int in_sq, t_minishell *shell)
{
	char	*val;

	if (arg[*i] == '$' && !in_sq && (arg[*i + 1] == '?'
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
	int		in_sq;
	int		in_dq;

	new = malloc(4096);
	if (!new)
		return (NULL);
	new[0] = '\0';
	i = 0;
	in_sq = 0;
	in_dq = 0;
	while (arg[i])
	{
		if (arg[i] == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (arg[i] == '\"' && !in_sq)
			in_dq = !in_dq;
		val = process_char(arg, &i, in_sq, shell);
		ft_strlcat(new, val, 4096);
		free(val);
	}
	return (new);
}

char	**resize_args(char **args, char *new_arg)
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
