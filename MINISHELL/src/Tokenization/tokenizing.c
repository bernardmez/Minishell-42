/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmazraan <bmazraan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:32:14 by jeid              #+#    #+#             */
/*   Updated: 2026/02/09 16:05:44 by bmazraan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*rediretions_token(t_cmd **cmd, char *prompt, t_env *env)
{
	int	len;
	int	type;

	while (*prompt != '\0')
	{
		type = redirections(*prompt, *(prompt + 1));
		if (type < 0)
			return (ft_error(&env, "error unmatched redirections", 130, false),
				NULL);
		if (type == 4 || type == 3)
			prompt += 2;
		else
			prompt++;
		prompt = skip_spaces(prompt);
		len = redirection_param(cmd, prompt, type, env);
		if (len == -1 || *prompt == '\0')
			return (NULL);
		prompt += len;
		if (*prompt != '\0')
			prompt = flags_token(cmd, prompt, env);
	}
	return (prompt);
}

/*
	@EXAMPLE arg1 -Flag1 arg2 -Flag2

	IF arg found skip the number of its characters
	else check if aynithing persist in prompt

*/
char	*args_token(t_cmd **cmd, int i, char *prompt, t_env *env)
{
	int	n;

	n = copy_args(cmd, prompt, env);
	if (n > 0)
		prompt += n;
	else
	{
		if (prompt[i] == '\0')
			return (NULL);
		prompt++;
	}
	return (prompt);
}

/*
	@EXAMPLE -FLAG1 -ARG1 -FLAG2 -ARG2 etc...

	LOOP (S):
		IF *S (-) OR *S IS (") but *S+1 IS (-):then
			copy all the following as flags
		ELSE:then
			copy all the rest as args
*/
char	*flags_token(t_cmd **cmd, char *prompt, t_env *env)
{
	int		i;
	char	*new_prompt;

	while (*prompt != '\0')
	{
		prompt = skip_spaces(prompt);
		i = 0;
		if (*prompt == '\0' || redirections(*prompt, *(prompt + 1)))
			break ;
		if (prompt[i] == '-' && !is_echo_with_flag(*cmd))
			prompt += copy_flag(cmd, i + 1, prompt, env);
		else if (isquote(prompt[i]) && prompt[i + 1] == '-'
			&& !is_echo_with_flag(*cmd))
			prompt += copy_flag(cmd, i + 2, prompt, env);
		else
		{
			new_prompt = args_token(cmd, i, prompt, env);
			if (!new_prompt)
				break ;
			prompt = new_prompt;
		}
	}
	return (prompt);
}

/*
	@EXAMPLE : ls -Flag1 -Flag2 arg1 arg2 -Flag3 > file1 file2 file3
	take the segments of commands splitted by pipes
	ex:segment[i] -> ls -Flag1 -Flag2 arg1 arg2 -Flag3 > file1 file2 file3
		-trim spaces
		-skip the prompt to the first space
		-calculate how many words skipped
			ls"test" = 8 chars
		-dequote the result and copy it
	@RETURN  -Flag1 -Flag2 arg1 arg2 -Flag3 > file1 file2 file3
*/
char	*command_token(t_cmd **cmd, char *prompt, t_env **env)
{
	char	*command;
	size_t	len;

	len = 0;
	prompt = skip_spaces(prompt);
	command = skip_to_c(prompt, ' ', *env);
	if (command == NULL)
	{
		ft_error(env, "parse error umatched quote`", 2, FALSE);
		return (free(command), NULL);
	}
	len = command - prompt;
	command = dequotencpy(0, len, prompt, env);
	if (*cmd != NULL)
		(*cmd)->command = command;
	else
		*cmd = struct_create_list(command, *cmd);
	return (prompt + len);
}

t_cmd	*tokenizing(char *prompt, t_env *env)
{
	t_cmd	*new_cmd;
	int		type;

	new_cmd = NULL;
	if (*prompt == '\0' || prompt == NULL)
		return (ft_error(&env, "parse error :invalid command", 2, FALSE), NULL);
	if (*prompt != '\0' && prompt != NULL)
		type = redirections(*prompt, *(prompt + 1));
	if (type != 0)
		prompt = check_redir(type, &env, &new_cmd, prompt);
	if (prompt == NULL || env->exit_status == 1)
		return (new_cmd);
	prompt = command_token(&new_cmd, skip_spaces(prompt), &env);
	if (env->exit_status == 1)
		return (new_cmd);
	if (*prompt != '\0')
		prompt = flags_token(&new_cmd, prompt, env);
	if (*prompt != '\0')
	{
		prompt = rediretions_token(&new_cmd, prompt, env);
		if (!prompt)
			return (struct_free_cmd(new_cmd), NULL);
	}
	return (new_cmd);
}
