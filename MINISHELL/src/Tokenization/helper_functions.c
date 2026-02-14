/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmazraan <bmazraan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:32:02 by jeid              #+#    #+#             */
/*   Updated: 2026/02/09 16:05:23 by bmazraan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*skip_quote_reset(char *outer_quote, char quote, char *s)
{
	*outer_quote = '\0';
	return (skip_inside(quote, s));
}

void	update_redirections(t_cmd **cmd, t_redir *new_redirection)
{
	t_redir	*temp;

	if ((*cmd)->redirections == NULL)
		(*cmd)->redirections = new_redirection;
	else
	{
		temp = (*cmd)->redirections;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_redirection;
	}
	return ;
}

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

static void	setup_heredoc(t_redir *redir, t_env **env)
{
	char	*num;
	int		*heredoc_count;

	heredoc_count = get_heredoc_count();
	num = ft_itoa((*heredoc_count)++);
	redir->heredoc_file = ft_strjoin("/tmp/heredoc_", num);
	free(num);
	handle_heredoc(env, redir, !(*env)->quote_indentifier);
}

int	redirection_param(t_cmd **cmd, char *prompt, int type, t_env *env)
{
	char		*filename;
	int			len;
	t_redir		*new_redirection;

	filename = skip_to_c(prompt, ' ', env);
	len = filename - prompt;
	if (len <= 0 || *prompt == '\0')
		return (ft_error(&env, "error expected filename", 130, false), -1);
	filename = dequotencpy(0, len, prompt, &env);
	if (*filename == '\0' || filename == NULL)
	{
		free(filename);
		return (ft_error(&env, "error expected filename", 130, false), -1);
	}
	new_redirection = (t_redir *)malloc(sizeof(t_redir));
	new_redirection->filename = filename;
	new_redirection->type = type;
	new_redirection->next = NULL;
	new_redirection->heredoc_file = NULL;
	if (type == 3)
		setup_heredoc(new_redirection, &env);
	update_redirections(cmd, new_redirection);
	return (len);
}

/*
	@EXAMPLE => "helo World this is a skip to c function"Code testing

	IF C IS ' ' (AKA space)
	@RETURN "helo World this is a skip to c function"Code
*/
char	*skip_to_c(char *s, char c, t_env *env)
{
	char	outer_quote;

	outer_quote = '\0';
	while (*s != '\0' && *s != c)
	{
		if (isquote(*s) && outer_quote == '\0')
			outer_quote = *s;
		else if (*s == outer_quote)
			outer_quote = '\0';
		if ((c != '|' && c != '$') && outer_quote == '\0' && redirections(*s,
				*(s + 1)) != 0)
			break ;
		if (!env->expanding && isquote(*s))
			s = skip_quote_reset(&outer_quote, *s, s + 1);
		else if (env->expanding && *s == '\'' && env->here_doc == FALSE)
			s = skip_inside(*s, s + 1);
		if ((env->here_doc || env->expanding) && s == NULL)
			return (NULL);
		if (s == NULL)
			return (ft_error(&env, "", 2, false), NULL);
		s++;
	}
	return (s);
}
