/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmazraan <bmazraan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 21:21:55 by bmazraan          #+#    #+#             */
/*   Updated: 2026/02/16 21:23:12 by bmazraan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	*get_heredoc_count(void)
{
	static int	heredoc_count = 0;

	return (&heredoc_count);
}

void	reset_heredoc_count(void)
{
	int	*count;

	count = get_heredoc_count();
	*count = 0;
}

void	setup_heredoc(t_redir *redir, t_env **env)
{
	char	*num;
	int		*heredoc_count;

	heredoc_count = get_heredoc_count();
	num = ft_itoa((*heredoc_count)++);
	redir->heredoc_file = ft_strjoin("/tmp/heredoc_", num);
	free(num);
	handle_heredoc(env, redir, !(*env)->quote_indentifier);
}

void	update_last_token(t_cmd **cmd, char *prompt, int len)
{
	int	last_start;

	free((*cmd)->last_token);
	if (ft_strncmp((*cmd)->command, "echo", 4) == 0)
	{
		last_start = len;
		while (last_start > 0 && prompt[last_start - 1] != ' ')
			last_start--;
		(*cmd)->last_token = ft_strndup(prompt + last_start, len - last_start);
	}
	else
		(*cmd)->last_token = ft_strndup(prompt, len);
}

int	copy_flag(t_cmd **cmd, int i, char *prompt, t_env *env)
{
	char	*flag;
	int		len;

	flag = skip_to_c(prompt, ' ', env);
	len = flag - prompt;
	env->flag = TRUE;
	flag = dequotencpy(i, len, prompt, &env);
	env->flag = FALSE;
	struct_update_flags(cmd, flag, (*cmd)->flag);
	free((*cmd)->last_token);
	(*cmd)->last_token = ft_strndup(prompt, len);
	return (len);
}
