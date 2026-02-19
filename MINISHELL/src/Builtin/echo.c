/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:28:52 by jeid              #+#    #+#             */
/*   Updated: 2026/02/09 21:22:04 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display(t_cmd **cmd)
{
	int	i;
	int	j;

	if ((*cmd)->arg == NULL)
	{
		printf("\n");
		return ;
	}
	i = -1;
	while ((*cmd)->arg[++i] != NULL)
	{
		j = 0;
		while ((*cmd)->arg[i][j] != '\0')
			printf("%c", (*cmd)->arg[i][j++]);
	}
}

bool	check_flag(char *flag)
{
	char	*tmp;

	tmp = flag + 1;
	while (*tmp)
	{
		if (*tmp != 'n')
		{
			printf("%s ", flag);
			return (TRUE);
		}
		tmp++;
	}
	return (FALSE);
}

void	echo_cmd(t_cmd **cmd, t_env **env)
{
	bool	new_line;

	new_line = TRUE;
	if ((*cmd)->flag)
		new_line = check_flag((*cmd)->flag);
	if ((*cmd)->arg == NULL && new_line)
	{
		printf("\n");
		if (env && *env)
			(*env)->exit_code = 0;
		return ;
	}
	display(cmd);
	if (new_line)
		printf("\n");
	if (env && *env)
		(*env)->exit_code = 0;
}
