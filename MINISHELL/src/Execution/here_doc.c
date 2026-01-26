/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:30:07 by jeid              #+#    #+#             */
/*   Updated: 2026/01/26 23:34:28 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int sig)
{
	(void)sig;
	g_signal = 130;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

void	signal_heredoc(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

char	**inside_while(t_env **env, char *eof, char **input_line, int fd)
{
	while (1)
	{
		signal_heredoc();
		(*env)->here_doc = TRUE;
		if (!eof || g_signal == 130)
			break ;
		input_line[0] = readline(">");
		if (input_line[0] == NULL || g_signal == 130)
		{
			free(input_line[0]);
			break ;
		}
		if ((*env)->quote_indentifier == FALSE)
			input_line = expansion(*env, input_line);
		if (is_delimiter(input_line[0], eof))
		{
			free(input_line[0]);
			break ;
		}
		write(fd, input_line[0], ft_strlen(input_line[0]));
		write(fd, "\n", 1);
		free(input_line[0]);
	}
	return (input_line);
}

static void	heredoc_child_process(t_env **env, char *eof)
{
	char	**input_line;
	int		fd;

	if (!eof)
		exit(STDIN_FILENO);
	fd = open("/tmp/heredoc_input", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		ft_error(env, "Error opening temporary file for heredoc", 1, false);
	input_line = malloc(sizeof(char *) * 2);
	input_line[1] = NULL;
	input_line = inside_while(env, eof, input_line, fd);
	(*env)->here_doc = FALSE;
	free(input_line);
	exit(0);
}

void	handle_heredoc(t_env **env, char *eof)
{
	int		pid;
	int		status;

	pid = fork();
	if (pid == -1)
		ft_error(env, "error fork in heredoc", 0, 0);
	if (pid == 0)
		heredoc_child_process(env, eof);
	else
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
			g_signal = 130;
		signals();
	}
}
