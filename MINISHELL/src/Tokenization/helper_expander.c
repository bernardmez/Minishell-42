/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmazraan <bmazraan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:31:57 by jeid              #+#    #+#             */
/*   Updated: 2026/02/09 16:05:17 by bmazraan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_expansion(t_expand *expander, char **segments)
{
	int	i;

	expander->section = ft_strdup(" ");
	expander->prev_section = NULL;
	i = 0;
	while (segments[i])
		i++;
	expander->expanded_segements = malloc(sizeof(char *) * (i + 1));
	return (-1);
}

char	*get_expand_value(t_env *env, char *key, int *i, char *rawvar)
{
	char	*value;

	if (*key == '?')
	{
		(*i)++;
		value = ft_itoa(env->exit_code);
	}
	else
		value = get_env_value(env, rawvar);
	return (value);
}

char	*expand(t_env *env, char *key)
{
	char	*expanded;
	char	*rawvar;
	char	*value;
	int		i;

	key++;
	rawvar = NULL;
	if (*key >= '0' && *key <= '9')
	{
		key++;
		return (ft_strdup(key));
	}
	i = 0;
	while (key[i] != '\0' && (ft_isalnum(key[i]) || key[i] == '_'))
		i++;
	rawvar = ft_strndup(key, i);
	value = get_expand_value(env, key, &i, rawvar);
	free(rawvar);
	key += i;
	if (value == NULL)
		return (ft_strdup(key));
	expanded = ft_strjoin(value, key);
	free(value);
	return (expanded);
}

void	expansion_mechanism(t_expand *expander, t_env *env)
{
	while (expander->section)
	{
		free(expander->section);
		expander->section = ft_strdup(skip_to_c(expander->next_section, '$',
					env));
		if (!expander->section || !*expander->section)
			break ;
		expander->len_section = skip_to_c(expander->next_section, '$', env)
			- expander->next_section;
		expander->var_value = expand(env, expander->section);
		expander->prev_section = ft_strndup(expander->next_section,
				expander->len_section);
		free(expander->next_section);
		expander->next_section = ft_strjoin(expander->prev_section,
				expander->var_value);
		free(expander->prev_section);
		free(expander->var_value);
	}
}
