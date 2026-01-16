/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_execute4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:29:53 by jeid              #+#    #+#             */
/*   Updated: 2026/01/15 21:29:54 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_and_check_path(char *dir, char *cmd)
{
	char	*part_path;
	char	*full_path;

	part_path = ft_strjoin(dir, "/");
	if (!part_path)
		return (NULL);
	full_path = ft_strjoin(part_path, cmd);
	free(part_path);
	if (!full_path)
		return (NULL);
	if (access(full_path, F_OK | X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

char	*handle_dot_slash(char *cmd)
{
	char	*cwd;
	char	*result;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	result = join_and_check_path(cwd, cmd + 2);
	free(cwd);
	return (result);
}

char	*find_path(char *cmd, t_env **env)
{
	char	**paths;
	char	*result;
	int		i;
	char	*env_path;

	if (ft_strncmp(cmd, "./", 2) == 0)
		return (handle_dot_slash(cmd));
	env_path = get_env_value(*env, "PATH");
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	free(env_path);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		result = join_and_check_path(paths[i], cmd);
		if (result)
			return (free_split(paths), result);
		i++;
	}
	free_split(paths);
	return (NULL);
}
