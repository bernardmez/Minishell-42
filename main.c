#include "minishell.h"

static t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->envp = envp;
	shell->cmds = NULL;
	shell->last_exit_status = 0;
	return (shell);
}

static void	print_cmds(t_cmd *cmds)
{
	t_cmd	*cmd;
	t_redir	*redir;
	int		i;

	cmd = cmds;
	while (cmd)
	{
		printf("Command:\n");
		i = 0;
		while (cmd->args && cmd->args[i])
		{
			printf("  arg[%d]: %s\n", i, cmd->args[i]);
			i++;
		}
		
		redir = cmd->redirs;
		while (redir)
		{
			printf("  Redirection: ");
			if (redir->type == TOKEN_REDIR_IN)
				printf("< %s\n", redir->file);
			else if (redir->type == TOKEN_REDIR_OUT)
				printf("> %s\n", redir->file);
			else if (redir->type == TOKEN_REDIR_APPEND)
				printf(">> %s\n", redir->file);
			else if (redir->type == TOKEN_REDIR_HEREDOC)
				printf("<< %s\n", redir->file);
			redir = redir->next;
		}
		
		if (cmd->next)
			printf("  | (pipe to next command)\n");
		printf("\n");
		cmd = cmd->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_shell	*shell;
	t_token	*tokens;
	t_cmd	*cmds;

	(void)argc;
	(void)argv;
	
	shell = init_shell(envp);
	if (!shell)
		return (1);
	
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		
		if (input[0] != '\0')
			add_history(input);
		
		if (!check_unclosed_quotes(input))
		{
			print_error("unclosed quotes");
			free(input);
			continue ;
		}
		
		tokens = lexer(input);
		if (!tokens)
		{
			free(input);
			continue ;
		}
		
		cmds = parser(tokens, shell);
		free_tokens(tokens);
		
		if (!cmds)
		{
			free(input);
			continue ;
		}
		
		print_cmds(cmds);
		
		
		free_cmds(cmds);
		free(input);
	}
	
	free(shell);
	return (0);
}