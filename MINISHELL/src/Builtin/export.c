/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:29:08 by jeid              #+#    #+#             */
/*   Updated: 2026/01/15 21:29:09 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// export function work :
// first check if '=' iclude the last word of variable
// next if yes split at '='
// finaly set_env the variable and the value
// or if not do nothing and return

int	check_valid(char *var, t_env **env)
{
	if (!(ft_isalpha(var[0]) || var[0] == '_'))
	{
		ft_error(env, "export: not an identifier", 1, false);
		return (1);
	}
	return (0);
}

void	free_name(char **name)
{
	int	j;

	j = 0;
	while (name[j])
	{
		free(name[j]);
		j++;
	}
	free(name);
	return ;
}

void	my_export(t_cmd **cmd, t_env **env)
{
	int		i;
	char	*equal_sign;
	char	**name;

	filter_export(cmd, env);
	if (!cmd || !(*cmd) || !(*cmd)->arg)
		return ;
	i = 0;
	while ((*cmd)->arg[i])
	{
		equal_sign = ft_strchr((*cmd)->arg[i], '=');
		if (equal_sign != NULL)
		{
			name = ft_split((*cmd)->arg[i], '=');
			if (check_valid(name[0], env))
			{
				free_name(name);
				return ;
			}
			set_env(name[0], equal_sign + 1, env);
			free_name(name);
		}
		i++;
	}
	return ;
}
