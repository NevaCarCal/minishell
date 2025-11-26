/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:05:00 by antigravity       #+#    #+#             */
/*   Updated: 2025/11/26 13:01:01 by ncarrera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	read_heredoc_loop(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
}

static int	handle_heredoc(char *delimiter)
{
	int		fd;

	fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	read_heredoc_loop(fd, delimiter);
	close(fd);
	fd = open(".heredoc_tmp", O_RDONLY);
	unlink(".heredoc_tmp");
	return (fd);
}

static int	open_file(char *file, int type)
{
	int	fd;

	if (type == REDIR_IN)
		fd = open(file, O_RDONLY);
	else if (type == REDIR_OUT)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == REDIR_APPEND)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (type == REDIR_HEREDOC)
		fd = handle_heredoc(file);
	else
		return (-1);
	if (fd == -1)
	{
		perror(file);
		return (-1);
	}
	return (fd);
}

int	handle_redirections(t_redir *redirs)
{
	int	fd;

	while (redirs)
	{
		fd = open_file(redirs->file, redirs->type);
		if (fd == -1)
			return (0);
		if (redirs->type == REDIR_IN || redirs->type == REDIR_HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redirs = redirs->next;
	}
	return (1);
}
