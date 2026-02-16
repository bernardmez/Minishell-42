/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmazraan <bmazraan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 21:21:21 by bmazraan          #+#    #+#             */
/*   Updated: 2026/02/16 21:21:25 by bmazraan         ###   ########.fr       */
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
