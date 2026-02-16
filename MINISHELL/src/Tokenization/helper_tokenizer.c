/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_tokenizer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmazraan <bmazraan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 21:22:12 by bmazraan          #+#    #+#             */
/*   Updated: 2026/02/16 21:23:18 by bmazraan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	example "ls -la | grep test"test hello world | grep test

	skips everything inside the quotations
	@RETURN test hello world | grep test
*/
char	*skip_inside(char quote, char *s)
{
	while (*s != '\0' && *s != quote)
		s++;
	if (*s == '\0')
		return (NULL);
	return (s);
}

/*
	@EXAMPLE "example on dequote and copy"TEST

	start = 0
	end =33
	s=@EXAMPLE

	create a variable with enough memorie address
	Track quotes and skip them
		eg: "example"
			=> " will be skipped as well the last one

	@RETURN	example on dequote and copyTEST\0
*/
char	*dequotencpy(int start, int end, char *s, t_env **env)
{
	int		i;
	int		j;
	char	*dest;
	char	in_quote;

	i = -1;
	j = 0;
	(*env)->quote_indentifier = FALSE;
	in_quote = '\0';
	dest = malloc(sizeof(char) * (end - start + 1));
	while (++i < (end - start) && s[start + i])
	{
		if (in_quote != '\0' && s[start + i] == in_quote)
			in_quote = '\0';
		else if (in_quote == '\0' && isquote(s[start + i]))
		{
			(*env)->quote_indentifier = TRUE;
			in_quote = s[start + i];
		}
		else
			dest[j++] = s[start + i];
	}
	if (in_quote != '\0' && !(*env)->flag)
		ft_error(env, "parse error unmatched quotes`", 2, FALSE);
	return (dest[j] = '\0', dest);
}

/*
	@EXAMPLE -ARG1 -FLAG2 -ARG2 > Filename

	check if the command is echo everything after we
		specify a flag is consider args
	except for redirections

	ELSE:then
		skip to the first space
		calc len word skiped
		copy without quotes
		update list of commands arguments
*/
int	copy_args(t_cmd **cmd, char *prompt, t_env *env)
{
	int		len;
	char	*argument;

	len = 0;
	if (ft_strncmp((*cmd)->command, "echo", 4) == 0)
	{
		len = find_arg_end_echo(prompt, &argument);
		argument = dequotencpy(0, len, prompt, &env);
		struct_update_args(cmd, argument);
	}
	else
	{
		argument = skip_to_c(prompt, ' ', env);
		if (argument == NULL)
			return (free(argument), 0);
		len = argument - prompt;
		argument = dequotencpy(0, len, prompt, &env);
		if (*argument == '\0' || env->exit_status == 1)
			return (free(argument), len);
		struct_update_args(cmd, argument);
	}
	return (len);
}

/*
	@EXAMPLE $HOME/test

	@RETURN /home/user/test
*/
char	**expansion(t_env *env, char **segments)
{
	int			i;
	t_expand	expander;
	char		*command;

	env->expanding = TRUE;
	i = init_expansion(&expander, segments);
	while (segments[++i])
	{
		command = skip_spaces(segments[i]);
		expander.next_section = ft_strdup(command);
		expansion_mechanism(&expander, env);
		expander.expanded_segements[i] = ft_strdup(expander.next_section);
		free(expander.next_section);
	}
	if (expander.section)
		free(expander.section);
	free_split(segments);
	expander.expanded_segements[i] = NULL;
	env->expanding = FALSE;
	return (expander.expanded_segements);
}

char	*check_redir(int type, t_env **env, t_cmd **cmd, char *prompt)
{
	if (type == 4 || type == 3)
		prompt += 2;
	else
		prompt++;
	*cmd = struct_create_list(NULL, *cmd);
	prompt = skip_spaces(prompt);
	prompt += redirection_param(cmd, skip_spaces(prompt), type, *env);
	if ((*env)->exit_status == 1)
		return (struct_free_cmd(*cmd), NULL);
	return (prompt);
}
