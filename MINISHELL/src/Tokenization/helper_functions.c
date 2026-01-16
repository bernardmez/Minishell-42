/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:32:02 by jeid              #+#    #+#             */
/*   Updated: 2026/01/15 21:41:26 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	redirection_param(t_cmd **cmd, char *prompt, int type, t_env *env)
{
	char	*filename;
	int		len;
	t_redir	*new_redirection;

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
	update_redirections(cmd, new_redirection);
	if (type == 3)
		handle_heredoc(&env, filename);
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
		if ((c != '|' && c != '$') && redirections(*s, *(s + 1)) != 0)
			break ;
		if (isquote(*s) && outer_quote == '\0')
			outer_quote = *s;
		else if (*s == outer_quote)
			outer_quote = '\0';
		if (!env->expanding && isquote(*s))
			s = skip_inside(*s, s + 1);
		else if (env->expanding && (*s == '\'' && outer_quote == '\'')
			&& env->here_doc == FALSE)
			s = skip_inside(*s, s + 1);
		if ((env->here_doc || env->expanding) && s == NULL)
			return (NULL);
		if (s == NULL)
			return (ft_error(&env, "", 2, false), NULL);
		s++;
	}
	return (s);
}

/*
	@EXAMPLE "-FLAG1"test  -ARG1 -ARG2 -FLAG2

	Skip s to find space taking quotes into considerations
		-calculate how many chars skipped
			"-FLAG1"test = 12 chars
		-dequote the result and copy it
	send new flag to the @upate_flags functions to concat new flags tokens

	@RETURN 12 chars(how many letter we read from
	prompt to be able to skip them with prompt+=len)
*/
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
	return (len);
}
