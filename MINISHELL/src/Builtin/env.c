/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:28:57 by jeid              #+#    #+#             */
/*   Updated: 2026/01/15 21:28:58 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_cmd(t_env **env)
{
	t_env	*current;

	current = *env;
	set_env("_", "/usr/bin/env", env);
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->variable_name, current->value);
		current = current->next;
	}
	return ;
}
