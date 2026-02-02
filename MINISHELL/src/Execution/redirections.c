/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:30:13 by jeid              #+#    #+#             */
/*   Updated: 2026/02/02 23:47:47 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_append(t_env **env, t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_error(env, "Append redirection failed", 1, false);
		return (STDOUT_FILENO);
	}
	return (fd);
}

int	handle_write(t_env **env, t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_error(env, "Output redirection failed", 1, false);
		return (STDOUT_FILENO);
	}
	return (fd);
}

int	handle_read_file(t_env **env, t_redir *redir, bool here_doc)
{
	int	fd;

	if (here_doc)
		fd = open(redir->heredoc_file, O_RDONLY);
	else
		fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		ft_error(env, "Input redirection failed", 0, false);
		return (STDIN_FILENO);
	}
	return (fd);
}

t_fd	handle_redirection(t_env **env, t_cmd *cmd)
{
	t_redir	*redir;
	t_fd	f;

	f.fd_1 = STDIN_FILENO;
	f.fd_2 = STDOUT_FILENO;
	redir = cmd->redirections;
	if (redir == NULL)
		return (f);
	while (redir)
	{
		if (redir->type == 1)
			f.fd_1 = handle_read_file(env, redir, FALSE);
		else if (redir->type == 2)
			f.fd_2 = handle_write(env, redir);
		else if (redir->type == 3)
		{
			f.fd_1 = handle_read_file(env, redir, TRUE);
		}
		else if (redir->type == 4)
			f.fd_2 = handle_append(env, redir);
		redir = redir->next;
	}
	return (f);
}
