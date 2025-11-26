/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 01:50:00 by antigravity       #+#    #+#             */
/*   Updated: 2025/11/26 02:55:09 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*remove_quotes(char *arg)
{
	char	*new;
	int		i;
	int		j;
	int		in_quote;
	char	quote;

	new = malloc(ft_strlen(arg) + 1);
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	in_quote = 0;
	quote = 0;
	while (arg[i])
	{
		if ((arg[i] == '\'' || arg[i] == '\"') && !in_quote)
		{
			in_quote = 1;
			quote = arg[i];
		}
		else if (in_quote && arg[i] == quote)
			in_quote = 0;
		else
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

static char	*expand_variables(char *arg, t_minishell *shell)
{
	char	*new;
	char	*val;
	int		i;
	int		k;
	char	*var;
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
		if (arg[i] == '$' && !in_sq && arg[i + 1] == '?')
		{
			val = ft_itoa(shell->exit_code);
			i += 2;
		}
		else if (arg[i] == '$' && !in_sq && (ft_isalnum(
					arg[i + 1]) || arg[i + 1] == '_'))
		{
			i++;
			k = i;
			while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
				i++;
			var = ft_strdup(arg + k);
			var[i - k] = '\0';
			val = get_env_val(var, shell);
			free(var);
		}
		else
		{
			val = malloc(2);
			val[0] = arg[i];
			val[1] = '\0';
			i++;
		}
		ft_strlcat(new, val, 4096);
		free(val);
	}
	return (new);
}

int	add_argument(t_command *cmd, char *arg, t_minishell *shell)
{
	int		i;
	char	**new_args;
	char	*expanded;
	char	*clean_arg;

	expanded = expand_variables(arg, shell);
	if (!expanded)
		return (0);
	clean_arg = remove_quotes(expanded);
	free(expanded);
	if (!clean_arg)
		return (0);
	i = 0;
	while (cmd->args && cmd->args[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
	{
		free(clean_arg);
		return (0);
	}
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = clean_arg;
	new_args[i + 1] = NULL;
	if (cmd->args)
		free(cmd->args);
	cmd->args = new_args;
	return (1);
}

int	add_redirection(t_command *cmd, t_redir_type type, char *file, t_minishell *shell)
{
	t_redir	*new;
	t_redir	*last;
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
	if (!cmd->redirs)
		cmd->redirs = new;
	else
	{
		last = cmd->redirs;
		while (last->next)
			last = last->next;
		last->next = new;
	}
	return (1);
}
