#include "minishell.h"

t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	add_cmd(t_cmd **head, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (!new_cmd)
		return ;
	if (!*head)
	{
		*head = new_cmd;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	t_redir	*redir_tmp;

	while (cmds)
	{
		tmp = cmds->next;
		free_array(cmds->args);
		while (cmds->redirs)
		{
			redir_tmp = cmds->redirs->next;
			free(cmds->redirs->file);
			free(cmds->redirs);
			cmds->redirs = redir_tmp;
		}
		free(cmds);
		cmds = tmp;
	}
}

static t_redir	*create_redir(t_token_type type, char *file)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	redir->next = NULL;
	return (redir);
}

static void	add_redir(t_redir **head, t_redir *new_redir)
{
	t_redir	*current;

	if (!new_redir)
		return ;
	if (!*head)
	{
		*head = new_redir;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

static int	count_args(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD)
			count++;
		else if (current->type != TOKEN_WORD)
			current = current->next;
		current = current->next;
	}
	return (count);
}

static t_cmd	*parse_simple_cmd(t_token **tokens)
{
	t_cmd	*cmd;
	t_token	*current;
	int		i;
	int		arg_count;

	cmd = create_cmd();
	if (!cmd)
		return (NULL);
	current = *tokens;
	arg_count = count_args(current);
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
		return (NULL);
	i = 0;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD)
		{
			cmd->args[i++] = ft_strdup(current->value);
			current = current->next;
		}
		else if (current->type == TOKEN_REDIR_IN 
			|| current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND 
			|| current->type == TOKEN_REDIR_HEREDOC)
		{
			add_redir(&cmd->redirs, create_redir(current->type, 
				current->next->value));
			current = current->next->next;
		}
		else
			current = current->next;
	}
	cmd->args[i] = NULL;
	*tokens = current;
	return (cmd);
}

t_cmd	*parser(t_token *tokens, t_shell *shell)
{
	t_cmd	*cmds;
	t_cmd	*cmd;
	t_token	*current;

	if (!tokens)
		return (NULL);
	
	if (!validate_syntax(tokens))
		return (NULL);
	
	expand_tokens(tokens, shell);
	
	cmds = NULL;
	current = tokens;
	while (current)
	{
		cmd = parse_simple_cmd(&current);
		if (!cmd)
		{
			free_cmds(cmds);
			return (NULL);
		}
		add_cmd(&cmds, cmd);
		
		if (current && current->type == TOKEN_PIPE)
			current = current->next;
	}
	return (cmds);
}