/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:31:11 by jeid              #+#    #+#             */
/*   Updated: 2026/01/15 21:31:12 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
	{
		free(args[i]);
		i++;
	}
	if (args != NULL)
		free(args);
}

/*
	ex: ls "HELLO |WORLD" | grep "test" | sort
	@RETURN number of commands=4
*/
int	ft_split_counts(char *s, char c, t_env *env)
{
	size_t	split_counts;

	split_counts = 0;
	s = skip_spaces(s);
	while (env->exit_status != 1 && *s)
	{
		if (*s != c)
		{
			s = skip_to_c(s, c, env);
			if (env->exit_status == 1)
			{
				printf("error: unmatched quotes `\n");
				return (split_counts);
			}
			split_counts++;
		}
		else
			s++;
	}
	return (split_counts);
}

/*
	ex:input => "ls -la | test" hello world | grep test

	malloc the tab //make a custome word count for more effieciency
	the usual ft_split but instead of looping threw the string here is the trick:
		we skip immidiatly to c while not taking into consideration anything
			-inside quotes
		and then we copy whetever we got

	example:
		str="ls -la | test" hello world | grep test
			skip to c = "ls -la | test" hello world
			copy this and repeat
		str=grep test
			skip to c= grep test //even if | is not found
			copy this ...
	find the manuals for skip to c for more details
*/
char	**ft_shell_split(char *s, char c, t_env *env)
{
	char	*word_start;
	char	**tabs;
	int		split_counts;
	size_t	i;

	split_counts = ft_split_counts(s, c, env);
	if (split_counts <= 0 || env->exit_status == 1)
		return (NULL);
	tabs = malloc((split_counts + 1) * sizeof(char *));
	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			word_start = s;
			s = skip_to_c(s, c, env);
			tabs[i] = ft_strndup(word_start, s - word_start);
			i++;
		}
		else
			s++;
	}
	tabs[i] = NULL;
	return (tabs);
}

char	**ft_split(char *s, char c)
{
	const char	*word_start;
	char		**tabs;
	size_t		i;

	tabs = malloc((ft_wordcount(s, c) + 1) * sizeof(char *));
	if (!s || !tabs)
		return (NULL);
	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			word_start = s;
			while (*s && *s != c)
				s++;
			tabs[i] = ft_strndup(word_start, s - word_start);
			if (!tabs[i])
				return (NULL);
			i++;
		}
		else
			s++;
	}
	tabs[i] = NULL;
	return (tabs);
}
