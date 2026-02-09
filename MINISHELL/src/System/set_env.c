/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmazraan <bmazraan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:31:44 by jeid              #+#    #+#             */
/*   Updated: 2026/02/09 16:06:14 by bmazraan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_envp(t_env *env)
{
	t_env	*temp;

	while (env != NULL)
	{
		temp = env;
		free(env->variable_name);
		free(env->value);
		env = env->next;
		free(temp);
	}
}

char	*get_env_value(t_env *env, char *key)
{
	if (!env)
		return (NULL);
	while (env)
	{
		if (ft_strcmp(env->variable_name, key) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (NULL);
}

void	init_envp(t_env **new_node)
{
	(*new_node)->flag = FALSE;
	(*new_node)->exit_code = 0;
	(*new_node)->expanding = FALSE;
	(*new_node)->here_doc = FALSE;
}

t_env	*save_envp(char **envp)
{
	t_env	*head;
	t_env	*temp;
	t_env	*new_node;
	char	**var_values;

	head = NULL;
	temp = NULL;
	while (*envp)
	{
		new_node = (t_env *)malloc(sizeof(t_env));
		init_envp(&new_node);
		var_values = ft_split(*envp, '=');
		new_node->variable_name = ft_strdup(var_values[0]);
		new_node->value = ft_strdup(var_values[1]);
		free_split(var_values);
		new_node->next = NULL;
		if (!head)
			head = new_node;
		else
			temp->next = new_node;
		temp = new_node;
		envp++;
	}
	return (head);
}

void	set_env(char *var, char *value, t_env **env)
{
	t_env	*curr;
	t_env	*new_node;

	curr = *env;
	while (curr)
	{
		if (ft_strncmp(curr->variable_name, var, ft_strlen(var) + 1) == 0)
		{
			free(curr->value);
			curr->value = ft_strdup(value);
			return ;
		}
		if (curr->next == NULL)
			break ;
		curr = curr->next;
	}
	new_node = (t_env *)malloc(sizeof(t_env));
	new_node->variable_name = ft_strdup(var);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	if (curr)
		curr->next = new_node;
	else
		*env = new_node;
}
