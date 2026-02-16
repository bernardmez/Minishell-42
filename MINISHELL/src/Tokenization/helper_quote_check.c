/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_quote_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmazraan <bmazraan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 21:22:02 by bmazraan          #+#    #+#             */
/*   Updated: 2026/02/16 21:23:16 by bmazraan         ###   ########.fr       */
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
