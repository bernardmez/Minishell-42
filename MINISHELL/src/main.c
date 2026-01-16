/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:32:21 by jeid              #+#    #+#             */
/*   Updated: 2026/01/15 21:52:37 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_signal = 0;

static void	process_input(char *input, t_env **env)
{
	t_cmd	*cmd;

	cmd = parsing(input, env);
	free(input);
	if (cmd && ((*env)->exit_status != 1 && g_signal != 130))
		executing(&cmd, env);
	struct_free_cmd(cmd);
}

void	start_engine(t_env *env)
{
	char	*input;

	while (1)
	{
		env->exit_status = 0;
		input = readline(PROMPT);
		if (input == NULL)
			handle_eof(&env);
		if (g_signal == 130)
			env->exit_code = g_signal;
		g_signal = 0;
		add_history(input);
		process_input(input, &env);
	}
}

/*
	Program Workflow

	input -> lexical analysis -> parser -> executing

	*input phase handle signals and user string
	*lexical analysis:
		-takes the whole input and degridate it to mutlitple commnads
			depending on pipes
		-each command then will be analysized to determine the:
			.command Name
			.flags
			.arguments
		-handle neccesary dequoting
	*parser phase pass all data and fetch environment to execute
*/

int	main(int argc, char *argv[], char *envp[])
{
	static t_env	*env;

	signals();
	(void)argv;
	(void)argc;
	env = save_envp(envp);
	increment_shlvl(&env);
	start_engine(env);
	return (free_envp(env), env->exit_code);
}
