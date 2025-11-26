/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 01:50:00 by antigravity       #+#    #+#             */
/*   Updated: 2025/11/26 13:18:13 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	should_copy_char(char c, int *in_quote, char *quote)
{
	if ((c == '\'' || c == '\"') && !*in_quote)
	{
		*in_quote = 1;
		*quote = c;
		return (0);
	}
	else if (*in_quote && c == *quote)
	{
		*in_quote = 0;
		return (0);
	}
	return (1);
}

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
		if (should_copy_char(arg[i], &in_quote, &quote))
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

static char	*get_expansion_val(char *arg, int *i, t_minishell *shell)
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

static char	*expand_variables(char *arg, t_minishell *shell)
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

static void	append_redir(t_command *cmd, t_redir *new)
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
	append_redir(cmd, new);
	return (1);
}
