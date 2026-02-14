/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 00:00:00 by jeid              #+#    #+#             */
/*   Updated: 2026/02/14 00:00:00 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_expand_special_cases(char *key)
{
	char	*result;

	if (*key == '\0' || (!ft_isalnum(*key) && *key != '_' && *key != '?'))
	{
		result = ft_strjoin("\x01", key);
		return (result);
	}
	if (*key >= '0' && *key <= '9')
	{
		key++;
		return (ft_strdup(key));
	}
	return (NULL);
}
