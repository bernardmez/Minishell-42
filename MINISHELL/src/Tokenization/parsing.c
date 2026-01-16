/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:32:21 by jeid              #+#    #+#             */
/*   Updated: 2026/01/15 21:52:37 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*lexical_analysis(t_env *env, char **segments)
{
	t_cmd	*cmd;
	t_cmd	*new_cmd;
	int		i;

	i = -1;
	cmd = NULL;
	new_cmd = NULL;
	(void)new_cmd;
	segments = expansion(env, segments);
	while (segments[++i] != NULL)
	{
		new_cmd = tokenizing(skip_spaces(segments[i]), env);
		if (!new_cmd || env->exit_status == 1)
		{
			struct_free_cmd(cmd);
			cmd = NULL;
			break ;
		}
		else
			struct_addback_list(&cmd, new_cmd);
	}
	free_split(segments);
	return (cmd);
}

t_cmd	*split_commands(char *input, t_env *env)
{
	t_cmd	*cmd;
	char	**segments;

	segments = ft_shell_split(input, '|', env);
	if (env->exit_status == 1 || segments == NULL)
		return (NULL);
	cmd = lexical_analysis(env, segments);
	return (cmd);
}

t_cmd	*parsing(char *input, t_env **env)
{
	t_cmd	*cmd;
	char	*n;
	int		i;

	cmd = NULL;
	input = skip_spaces(input);
	if (!input || *input == '\0')
		return (NULL);
	i = 0;
	while (input[i])
	{
		n = skip_spaces(input + (i + 1));
		if (input[0] == '|' || (i > 0 && ((*n == '|' || *n == '\0')
					&& input[i] == '|')))
			return (ft_error(env, "parse error near `|'", 2, false), NULL);
		i++;
	}
	if (skip_to_c(input, '\0', *env) == NULL)
		return (ft_error(env, "parse error unmatched quotes`", 2, false), NULL);
	if ((*env)->exit_status != 1)
		cmd = split_commands(input, *env);
	return (cmd);
}
