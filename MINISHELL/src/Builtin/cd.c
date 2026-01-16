/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:28:45 by jeid              #+#    #+#             */
/*   Updated: 2026/01/15 21:28:46 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*build_full_path(char *current_pwd, char *arg)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(current_pwd, "/");
	free(current_pwd);
	full_path = ft_strjoin(temp, arg);
	free(temp);
	return (full_path);
}

static void	update_pwd_env(t_env **env, char *new_pwd, char *current_pwd)
{
	if (new_pwd)
		set_env("PWD", new_pwd, env);
	else if (current_pwd)
		set_env("PWD", current_pwd, env);
	else
		ft_error(env, "bash: cd: getcwd failed", 1, false);
}

void	update_pwd(t_cmd **cmd, t_env **env)
{
	char	*new_pwd;
	char	*current_pwd;

	if (!cmd || !(*cmd))
		return ;
	new_pwd = getcwd(NULL, 0);
	current_pwd = get_env_value(*env, "PWD");
	if ((*cmd)->arg && (*cmd)->arg[0])
		current_pwd = build_full_path(current_pwd, (*cmd)->arg[0]);
	update_pwd_env(env, new_pwd, current_pwd);
	if (new_pwd)
		free(new_pwd);
	if (current_pwd)
		free(current_pwd);
}

void	cd_cmd(t_cmd **cmd, t_env **env)
{
	char	*old_pwd;
	char	*current_pwd;

	if (!cmd || !(*cmd))
		return ;
	old_pwd = getcwd(NULL, 0);
	current_pwd = get_env_value(*env, "PWD");
	if (old_pwd != NULL)
		set_env("OLDPWD", old_pwd, env);
	else
		set_env("OLDPWD", current_pwd, env);
	if (current_pwd)
		free(current_pwd);
	if (old_pwd)
		free(old_pwd);
	if ((*cmd)->arg == NULL || (*cmd)->arg[0] == NULL)
	{
		if (chdir(getenv("HOME")) == -1)
			ft_error(env, "bash: cd: getcwd failed", 1, false);
	}
	else if (chdir((*cmd)->arg[0]) == -1)
		ft_error(env, "error no such file or directiory", 1, false);
	update_pwd(cmd, env);
}
