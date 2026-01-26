/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 23:59:00 by jeid              #+#    #+#             */
/*   Updated: 2026/01/26 23:52:08 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_export_identifier_error(t_env **env, const char *token)
{
	(void)env;
	write(STDERR_FILENO, "export: not an identifier: ", 28);
	if (token)
		write(STDERR_FILENO, token, ft_strlen((char *)token));
	write(STDERR_FILENO, "\n", 1);
	if (env && *env)
		(*env)->exit_code = 1;
}

void	copy_name_before_equal(char *dst, size_t dstsz, const char *arg)
{
	size_t	i;

	if (!dst || dstsz == 0)
		return ;
	i = 0;
	while (arg && arg[i] && arg[i] != '=' && i + 1 < dstsz)
	{
		dst[i] = arg[i];
		i++;
	}
	dst[i] = '\0';
}

int	is_assign_valid(char *arg, t_env **env)
{
	char	*eq;
	char	*name;
	int		ret;

	if (!arg)
		return (0);
	eq = ft_strchr(arg, '=');
	if (!eq)
		return (0);
	name = ft_strndup(arg, (size_t)(eq - arg));
	if (!name)
	{
		ft_error(env, "malloc", 1, false);
		return (0);
	}
	ret = !check_valid(name, env);
	free(name);
	return (ret);
}
