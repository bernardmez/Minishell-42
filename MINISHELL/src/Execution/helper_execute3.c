/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_execute3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:29:49 by jeid              #+#    #+#             */
/*   Updated: 2026/01/15 21:29:50 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_size(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		if (env->variable_name && env->value)
			size++;
		env = env->next;
	}
	return (size);
}

static void	free_string_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static char	**env_to_envp(t_env *env)
{
	char	**envp;
	char	*tmp;
	int		i;

	i = 0;
	envp = malloc(sizeof(char *) * (env_size(env) + 1));
	if (!envp)
		return (NULL);
	while (env)
	{
		if (env->variable_name && env->value)
		{
			tmp = ft_strjoin(env->variable_name, "=");
			envp[i++] = ft_strjoin_free(tmp, env->value);
		}
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}

static char	**build_exec_args(t_cmd *cmd)
{
	char	**args;
	int		i;
	int		j;

	i = 1;
	j = 0;
	args = malloc(sizeof(char *) * (cmd->arg_number + 3));
	if (!args)
	{
		free(args);
		return (NULL);
	}
	args[0] = cmd->command;
	while (j < cmd->arg_number)
		args[i++] = cmd->arg[j++];
	if (cmd->flag && *cmd->flag)
		args[i++] = cmd->flag;
	args[i] = NULL;
	return (args);
}

int	execute(char *path, t_cmd **cmd, t_env **env)
{
	char	**args;
	char	**cenvp;

	args = build_exec_args(*cmd);
	if (!args)
		ft_error(env, "malloc failed", 1, false);
	cenvp = env_to_envp(*env);
	if (!cenvp)
	{
		free(args);
		ft_error(env, "env error", 1, false);
	}
	if (execve(path, args, cenvp) == -1)
	{
		free_string_array(cenvp);
		free(args);
		ft_error(env, "42-minishell: error command not found", 126, false);
		return (-1);
	}
	return (1);
}
