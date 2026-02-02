/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_execute6.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 00:15:00 by jeid              #+#    #+#             */
/*   Updated: 2026/02/03 00:15:42 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sigint_wait_handler(int sig)
{
	(void)sig;
	g_signal = 130;
}

static int	reap_remaining_children(void)
{
	int	tmp;
	int	sigint;

	sigint = 0;
	while (waitpid(-1, &tmp, 0) > 0)
		if (WIFSIGNALED(tmp) && WTERMSIG(tmp) == SIGINT)
			sigint = 1;
	return (sigint);
}

static void	handle_exit_status(int status, t_env **env, int *sigint)
{
	if (WIFEXITED(status))
		(*env)->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		(*env)->exit_code = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(2, "Quit (core dumped)\n", 19);
		if (WTERMSIG(status) == SIGINT)
			*sigint = 1;
	}
}

void	wait_for_children(pid_t pid, t_env **env, t_cmd **cmd)
{
	int	status;
	int	sigint;

	(void)cmd;
	sigint = 0;
	signal(SIGINT, sigint_wait_handler);
	waitpid(pid, &status, 0);
	if (reap_remaining_children())
		sigint = 1;
	handle_exit_status(status, env, &sigint);
	if (sigint || g_signal == 130)
	{
		write(1, "\n", 1);
		(*env)->exit_code = 130;
	}
	g_signal = 0;
	signals();
}
