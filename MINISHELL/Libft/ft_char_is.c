/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_char_is.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:30:24 by jeid              #+#    #+#             */
/*   Updated: 2026/01/15 21:30:25 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	isquote(char c)
{
	return (c == '`' || c == '\'' || c == '\"');
}

int	redirections(char c1, char c2)
{
	if (c1 == '>' && c2 == '<')
		return (-1);
	if (c1 == '<' && c2 == '>')
		return (-1);
	if (c1 == '>' && c2 == '>')
		return (4);
	if (c1 == '<' && c2 == '<')
		return (3);
	if (c1 == '>')
		return (2);
	if (c1 == '<')
		return (1);
	return (0);
}

int	isprintable(char c)
{
	return (c >= 32 && c <= 126);
}

int	ft_isalnum(int c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0'
			&& c <= '9'));
}

int	ft_isalpha(int c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}
