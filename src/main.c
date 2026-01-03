#include "minishell.h"

static t_shell *init_shell(char **envp)
{
	t_shell *shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->envp = copy_envp(envp);
	if (!shell->envp)
	{
		free(shell);
		return (NULL);
	}
	shell->cmds = NULL;
	shell->last_exit_status = 0;
	return (shell);
}

int main(int argc, char **argv, char **envp)
{
	char *input;
	t_shell *shell;
	t_token *tokens;
	t_cmd *cmds;

	(void)argc;
	(void)argv;

	shell = init_shell(envp);
	if (!shell)
		return (1);

	setup_signals();

	while (1)
	{
		char *prompt;

		g_signal = 0;
		if (isatty(STDIN_FILENO))
		{
			prompt = get_prompt(shell);
			input = readline(prompt);
			free(prompt);
		}
		else
		{
			char buffer[4096];
			if (fgets(buffer, sizeof(buffer), stdin) == NULL)
				break;
			input = ft_strdup(buffer);
			if (input && ft_strlen(input) > 0 && input[ft_strlen(input) - 1] == '\n')
				input[ft_strlen(input) - 1] = '\0';
		}
		if (!input)
		{
			if (isatty(STDIN_FILENO))
				write(1, "exit\n", 5);
			break;
		}

		if (input[0] != '\0')
			add_history(input);

		if (!check_unclosed_quotes(input))
		{
			print_error("unclosed quotes");
			shell->last_exit_status = 1;
			free(input);
			continue;
		}

		tokens = lexer(input);
		if (!tokens)
		{
			free(input);
			continue;
		}

		cmds = parser(tokens, shell);
		free_tokens(tokens);

		if (!cmds)
		{
			// If tokens existed but cmds is NULL, it's a syntax error
			shell->last_exit_status = 2;
			free(input);
			continue;
		}

		shell->last_exit_status = execute_pipeline(cmds, shell);

		free_cmds(cmds);
		free(input);
	}

	free_envp(shell->envp);
	free(shell);
	return (0);
}
