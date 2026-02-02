/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_quote_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 00:00:00 by jeid              #+#    #+#             */
/*   Updated: 2026/02/02 23:05:40 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_echo_with_flag(t_cmd *cmd)
{
	return (ft_strncmp(cmd->command, "echo", 4) == 0 && cmd->flag != NULL);
}

static int	check_redir_outside_quotes(char *str, char in_quote)
{
	if (in_quote == '\0' && redirections(*str, *(str + 1)))
		return (1);
	return (0);
}

int	find_arg_end_echo(char *prompt, char **argument)
{
	char	in_quote;

	*argument = skip_spaces(prompt);
	in_quote = '\0';
	while (**argument != '\0')
	{
		if (isquote(**argument) && in_quote == '\0')
			in_quote = **argument;
		else if (**argument == in_quote)
			in_quote = '\0';
		else if (check_redir_outside_quotes(*argument, in_quote))
			break ;
		(*argument)++;
	}
	return (*argument - prompt);
}
