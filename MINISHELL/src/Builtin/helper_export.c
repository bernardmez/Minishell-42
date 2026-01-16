/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:29:13 by jeid              #+#    #+#             */
/*   Updated: 2026/01/15 21:29:14 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_len(t_env *env)
{
	int	len;

	len = 0;
	while (env)
	{
		len++;
		env = env->next;
	}
	return (len);
}

void	sort_env_array(t_env **env_arr)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (env_arr[i])
	{
		j = i + 1;
		while (env_arr[j])
		{
			if (ft_strcmp(env_arr[i]->variable_name,
					env_arr[j]->variable_name) > 0)
			{
				tmp = env_arr[i];
				env_arr[i] = env_arr[j];
				env_arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

t_env	**env_to_array(t_env *env)
{
	t_env	**arr;
	int		len;
	int		i;

	if (!env)
		return (NULL);
	len = env_len(env);
	arr = malloc(sizeof(t_env *) * (len + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		arr[i++] = env;
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}

void	filter_export(t_cmd **cmd, t_env **env)
{
	t_env	**env_arr;
	int		i;

	if (!(*cmd)->arg || !(*cmd)->arg[0])
	{
		if (!env)
			return ;
		env_arr = env_to_array(*env);
		if (!env_arr)
			return ;
		sort_env_array(env_arr);
		i = 0;
		while (env_arr[i])
		{
			if (env_arr[i]->value)
				printf("declare -x %s=\"%s\"\n",
					env_arr[i]->variable_name, env_arr[i]->value);
			else
				printf("declare -x %s\n", env_arr[i]->variable_name);
			i++;
		}
		free(env_arr);
	}
}
