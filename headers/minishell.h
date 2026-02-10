/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 13:07:03 by ncarrera          #+#    #+#             */
/*   Updated: 2026/02/10 20:48:24 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <errno.h>
# include "../libft/libft.h"

extern int	g_signal;

typedef struct s_minishell
{
	char	**envp;
	int		exit_code;
	int		exit_loop;
}	t_minishell;

typedef struct s_parser_state
{
	int		in_quote;
	char	quote;
	int		len;
}	t_parser_state;

typedef struct s_parse_info
{
	char		*line;
	int			*i;
	t_minishell	*shell;
}	t_parse_info;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_command
{
	char				**args;
	t_redir				*redirs;
	struct s_command	*next;
}	t_command;

/* Utils */
char		*get_next_token(char *line, int *idx);
int			is_operator(char c);
t_command	*new_command(void);
int			add_argument(t_command *cmd, char *arg, t_minishell *shell);
int			add_redirection(t_command *cmd, t_redir_type type,
				char *file, t_minishell *shell);
t_redir		*new_redir(t_redir_type type, char *file);

/* Parser */
t_command	*parse_input(char *line, t_minishell *shell);
void		free_commands(t_command *cmds);
char		*remove_quotes(char *arg);
char		*get_expansion_val(char *arg, int *i, t_minishell *shell);
char		*expand_variables(char *arg, t_minishell *shell);
char		**resize_args(char **args, char *new_arg);

/* Executor */
void		execute_command(t_minishell *shell, t_command *cmd);
void		exec_child(t_command *cmd,
				t_minishell *shell, int prev_fd, int *pip);
void		handle_parent(int *prev_fd, int *pip);
void		clean_empty_arg(t_command *cmd);
void		exec_single_builtin(t_command *cmd, t_minishell *shell);
char		*find_path(char *cmd, char **envp);

/* Shell */
void		process_line(char *line, t_minishell *shell);
int			init_shell(t_minishell *shell, char **envp, int argc);
void		loop_shell(t_minishell *shell);
void		free_env(char **envp);

/* Builtins */
int			is_builtin(char *cmd);
void		exec_builtin(t_minishell *shell, char **args);
void		builtin_cd(t_minishell *shell, char **args);
void		builtin_pwd(t_minishell *shell);
void		builtin_env(t_minishell *shell);
void		builtin_exit(t_minishell *shell, char **args);
void		builtin_echo(t_minishell *shell, char **args);
void		builtin_export(t_minishell *shell, char **args);
void		builtin_unset(t_minishell *shell, char **args);
int			env_var_exists(char *var, char **envp);

/* Executor Utils */
char		*check_absolute_path(char *cmd);
void		handle_exec_error(char *cmd, t_minishell *shell);
void		wait_for_child(pid_t pid, t_minishell *shell);
char		*join_path(char *dir, char *cmd);
int			handle_redirections(t_redir *redirs);

#endif
