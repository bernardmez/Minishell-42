/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:29:22 by jeid              #+#    #+#             */
/*   Updated: 2026/01/15 21:29:23 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_current(t_env *current)
{
	if (!current)
		return ;
	free(current->variable_name);
	free(current->value);
	free(current);
}

void	unset_helper(t_cmd **cmd, t_env **env, t_env *current, t_env *prev)
{
	if (ft_strncmp((*cmd)->arg[0], "_", 1) == 0)
		return ;
	if (prev)
		prev->next = current->next;
	else
		*env = current->next;
	free_current(current);
}

void	my_unset(t_cmd **cmd, t_env **env)
{
	size_t	len;
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	if ((*cmd)->arg == NULL)
		return ;
	len = ft_strlen((*cmd)->arg[0]);
	while (current)
	{
		if (ft_strncmp(current->variable_name, (*cmd)->arg[0], len) == 0)
		{
			unset_helper(cmd, env, current, prev);
			break ;
		}
		prev = current;
		current = current->next;
	}
}
