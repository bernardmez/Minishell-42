#include "minishell.h"

static int	is_redir_token(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HEREDOC);
}

int	validate_syntax(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (1);
	current = tokens;
	
	if (current->type == TOKEN_PIPE)
	{
		syntax_error("|");
		return (0);
	}
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (!current->next || current->next->type == TOKEN_PIPE)
			{
				syntax_error("|");
				return (0);
			}
		}
		if (is_redir_token(current->type))
		{
			if (!current->next || current->next->type != TOKEN_WORD)
			{
				if (current->next)
					syntax_error(current->next->value);
				else
					syntax_error("newline");
				return (0);
			}
		}
		current = current->next;
	}
	current = tokens;
	while (current->next)
		current = current->next;
	if (is_redir_token(current->type))
	{
		syntax_error("newline");
		return (0);
	}
	return (1);
}