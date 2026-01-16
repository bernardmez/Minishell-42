/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:29:03 by jeid              #+#    #+#             */
/*   Updated: 2026/01/15 21:29:04 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	int	tmp;

	if ((*cmd)->arg_number == 0)
		exit_t(cmd, env, (*env)->exit_code);
	if (!ft_is_numeric((*cmd)->arg[0]))
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
	tmp = ft_atoi((*cmd)->arg[0]);
	exit_t(cmd, env, tmp);
}
