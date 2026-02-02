/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_execute2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:29:45 by jeid              #+#    #+#             */
/*   Updated: 2026/02/03 00:15:42 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_pipe(t_env **env, int fd[2])
{
	if (pipe(fd) == -1)
		ft_error(env, "error cannot create pipe", 1, true);
	return (0);
}

pid_t	create_process(t_env **env)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		ft_error(env, "error system sub process", 1, true);
	return (pid);
}

void	handle_child_process(t_cmd **cmd, t_pipe pipe_fd, t_env **env)
{
	if ((*cmd)->next != NULL)
	{
		close(pipe_fd.fd[0]);
		if (dup2(pipe_fd.fd[1], STDOUT_FILENO) == -1)
			ft_error(env, "error system sub process", 1, true);
		close(pipe_fd.fd[1]);
	}
	if (pipe_fd.saved_fd != STDIN_FILENO)
	{
		if (dup2(pipe_fd.saved_fd, STDIN_FILENO) == -1)
			ft_error(env, "error system sub process", 1, true);
		close(pipe_fd.saved_fd);
	}
	check_cmd(cmd, env);
	ft_error(env, "", 0, true);
}

void	handle_parent_process(t_pipe *pipe_fd, t_cmd **cmd)
{
	if (pipe_fd->saved_fd != STDIN_FILENO)
		close(pipe_fd->saved_fd);
	if ((*cmd)->next != NULL)
	{
		close(pipe_fd->fd[1]);
		pipe_fd->saved_fd = pipe_fd->fd[0];
	}
}
