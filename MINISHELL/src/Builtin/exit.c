/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:29:03 by jeid              #+#    #+#             */
/*   Updated: 2026/02/09 20:09:22 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>
#include <errno.h>

void	exit_t(t_cmd **cmd, t_env **env, int tmp)
{
	struct_free_cmd(*cmd);
	free_envp(*env);
	printf("Exiting minishell...\n");
	exit(tmp);
}

int	ft_is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[0] == '+' || str[0] == '-')
		i++;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

void	exit_minishell(t_cmd **cmd, t_env **env)
{
	long		num;
	char		*endptr;
	int			is_numeric;
	int			exit_code;

	if ((*cmd)->arg_number == 0)
		exit_t(cmd, env, (*env)->exit_code);
	errno = 0;
	is_numeric = ft_is_numeric((*cmd)->arg[0]);
	num = strtol((*cmd)->arg[0], &endptr, 10);
	if (!is_numeric || *endptr != '\0' || errno == ERANGE)
	{
		printf("minishell: exit: %s: numeric argument required\n",
			(*cmd)->arg[0]);
		exit_t(cmd, env, 2);
	}
	if ((*cmd)->arg_number > 1)
	{
		printf("minishell: exit: too many arguments\n");
		(*env)->exit_code = 1;
		return ;
	}
	exit_code = ((num % 256) + 256) % 256;
	exit_t(cmd, env, exit_code);
}
