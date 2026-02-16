/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:29:33 by jeid              #+#    #+#             */
/*   Updated: 2026/02/09 22:27:18 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_dup2(t_fd ff)
{
	if (ff.fd_1 != STDIN_FILENO)
		dup2(ff.fd_1, STDIN_FILENO);
	if (ff.fd_2 != STDOUT_FILENO)
		dup2(ff.fd_2, STDOUT_FILENO);
}

char	*relative_path(t_cmd **cmd, t_env **env)
{
	char	*path;

	path = NULL;
	if (ft_strchr((*cmd)->command, '/') == NULL)
		path = find_path((*cmd)->command, env);
	else
		path = (*cmd)->command;
	return (path);
}

void	check_cmd(t_cmd **cmd, t_env **env)
{
	pid_t	pid;
	int		check_builtins;
	t_fd	ff;
	char	*path;

	ff = handle_redirection(env, *cmd);
	pid = fork();
	if (pid == 0)
	{
		handle_dup2(ff);
		check_builtins = built_in_functions(cmd, env);
		if (check_builtins)
		{
			path = relative_path(cmd, env);
			if (!path)
				ft_error(env, ft_strjoin((*cmd)->command, "command not found"),
					127, true);
			restoresignal();
			execute(path, cmd, env);
		}
		ft_error(env, "", 1, true);
	}
	else
		wait_for_children(pid, env, cmd);
}

void	execute_simple_cmd(t_cmd **cmd, t_env **env)
{
	t_fork	pipe;
	int		check_builtins;

	if (save_original_fds(&pipe.original_fds.fd_1, &pipe.original_fds.fd_2,
			env) == -1)
		return ;
	pipe.ff = handle_redirection(env, *cmd);
	handle_dup2(pipe.ff);
	check_builtins = built_in_functions(cmd, env);
	if (check_builtins)
		inside_fork(pipe, env, cmd);
	restore_original_fds(pipe.original_fds.fd_1, pipe.original_fds.fd_2, env);
}

void	executing(t_cmd **cmd, t_env **env)
{
	t_pipe	pipe_fd;
	pid_t	pid;
	t_cmd	*last_cmd;

	if ((*cmd)->next == NULL)
	{
		execute_simple_cmd(cmd, env);
		set_env("_", get_last_arg(*cmd), env);
		return ;
	}
	pipe_fd.saved_fd = dup(STDIN_FILENO);
	last_cmd = NULL;
	pid = execute_pipeline_loop(cmd, &pipe_fd, env, &last_cmd);
	wait_for_children(pid, env, cmd);
	if (last_cmd)
		set_env("_", get_last_arg(last_cmd), env);
}
