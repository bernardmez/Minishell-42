/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:29:08 by jeid              #+#    #+#             */
/*   Updated: 2026/01/26 23:52:08 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// export function work :
// first check if '=' iclude the last word of variable
// next if yes split at '='
// finaly set_env the variable and the value
// or if not do nothing and return

/* moved to export_utils.c */

int	check_valid(char *var, t_env **env)
{
	int	i;

	if (!var || !var[0])
	{
		print_export_identifier_error(env, "");
		return (1);
	}
	if (!(ft_isalpha(var[0]) || var[0] == '_'))
	{
		print_export_identifier_error(env, var);
		return (1);
	}
	i = 1;
	while (var[i])
	{
		if (!(ft_isalnum(var[i]) || var[i] == '_'))
		{
			print_export_identifier_error(env, var);
			return (1);
		}
		i++;
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

static void	export_assign(char *arg, char *equal_sign, t_env **env)
{
	char	**name;

	(void)equal_sign;
	name = ft_split(arg, '=');
	if (!name)
		return ;
	if (!check_valid(name[0], env))
		set_env(name[0], ft_strchr(arg, '=') + 1, env);
	free_name(name);
}

static void	export_name(char *arg, t_env **env)
{
	if (!check_valid(arg, env))
		set_env(arg, NULL, env);
}

void	my_export(t_cmd **cmd, t_env **env)
{
	int		i;
	char	*equal_sign;

	filter_export(cmd, env);
	if (!cmd || !(*cmd) || !(*cmd)->arg)
		return ;
	i = 0;
	while ((*cmd)->arg[i])
	{
		equal_sign = ft_strchr((*cmd)->arg[i], '=');
		if (equal_sign)
		{
			if (!is_assign_valid((*cmd)->arg[i], env))
				break ;
			export_assign((*cmd)->arg[i], equal_sign, env);
		}
		else
		{
			if (check_valid((*cmd)->arg[i], env))
				break ;
			export_name((*cmd)->arg[i], env);
		}
		i++;
	}
}
