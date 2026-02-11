/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 13:35:59 by ncarrera          #+#    #+#             */
/*   Updated: 2026/02/11 20:17:45 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	should_copy_char(char c, char *quote)
{
	char	prev_quote;

	prev_quote = *quote;
	ft_update_quote(c, quote);
	if (*quote != prev_quote)
		return (0);
	return (1);
}

char	*remove_quotes(char *arg)
{
	char	*new;
	int		i;
	int		j;
	char	quote;

	new = malloc(ft_strlen(arg) + 1);
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (arg[i])
	{
		if (should_copy_char(arg[i], &quote))
			new[j++] = arg[i];
		i++;
	}
	new[j] = '\0';
	return (new);
}

static char	*get_env_val(char *var, t_minishell *shell)
{
	int		i;
	int		len;
	char	*val;

	if (ft_strncmp(var, "?", 2) == 0)
		return (ft_itoa(shell->exit_code));
	i = 0;
	len = ft_strlen(var);
	while (shell->envp[i])
	{
		if (ft_strncmp(
				shell->envp[i], var, len) == 0 && shell->envp[i][len] == '=')
		{
			val = ft_strdup(shell->envp[i] + len + 1);
			return (val);
		}
		i++;
	}
	return (ft_strdup(""));
}

static char	*get_var_value(char *arg, int *i, t_minishell *shell)
{
	int		k;
	char	*var;
	char	*val;

	(*i)++;
	k = *i;
	while (arg[*i] && (ft_isalnum(arg[*i]) || arg[*i] == '_'))
		(*i)++;
	var = ft_strdup(arg + k);
	var[*i - k] = '\0';
	val = get_env_val(var, shell);
	free(var);
	return (val);
}

char	*get_expansion_val(char *arg, int *i, t_minishell *shell)
{
	if (arg[*i + 1] == '?')
	{
		*i += 2;
		return (ft_itoa(shell->exit_code));
	}
	if (ft_isalnum(arg[*i + 1]) || arg[*i + 1] == '_')
		return (get_var_value(arg, i, shell));
	return (NULL);
}
