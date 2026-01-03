#include "minishell.h"

t_token	*create_token(t_token_type type, char *value, t_quote_state quote)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->quote_state = quote;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

static int	is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static int	handle_quotes(char *input, int *i, char **word, t_quote_state *quote)
{
	char	quote_char;
	int		start;
	char	*temp;

	quote_char = input[*i];
	*quote = (quote_char == '\'') ? QUOTE_SINGLE : QUOTE_DOUBLE;
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;
	if (!input[*i])
		return (0);
	temp = ft_substr(input, start, *i - start);
	if (*word)
	{
		*word = ft_strjoin(*word, temp);
		free(temp);
	}
	else
		*word = temp;
	(*i)++;
	return (1);
}

static char	*extract_word(char *input, int *i, t_quote_state *quote)
{
	char	*word;
	int		start;
	char	*temp;

	word = NULL;
	*quote = QUOTE_NONE;
	while (input[*i] && !ft_isspace(input[*i]) && !is_metachar(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			if (!handle_quotes(input, i, &word, quote))
			{
				free(word);
				return (NULL);
			}
		}
		else
		{
			start = *i;
			while (input[*i] && !ft_isspace(input[*i]) 
				&& !is_metachar(input[*i]) && input[*i] != '\'' && input[*i] != '"')
				(*i)++;
			temp = ft_substr(input, start, *i - start);
			if (word)
			{
				word = ft_strjoin(word, temp);
				free(temp);
			}
			else
				word = temp;
		}
	}
	return (word);
}

static t_token	*handle_redir(char *input, int *i)
{
	t_token_type	type;
	char			*value;

	if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			type = TOKEN_REDIR_HEREDOC;
			value = ft_strdup("<<");
			(*i) += 2;
		}
		else
		{
			type = TOKEN_REDIR_IN;
			value = ft_strdup("<");
			(*i)++;
		}
	}
	else
	{
		if (input[*i + 1] == '>')
		{
			type = TOKEN_REDIR_APPEND;
			value = ft_strdup(">>");
			(*i) += 2;
		}
		else
		{
			type = TOKEN_REDIR_OUT;
			value = ft_strdup(">");
			(*i)++;
		}
	}
	return (create_token(type, value, QUOTE_NONE));
}

t_token	*lexer(char *input)
{
	t_token			*tokens;
	t_token			*token;
	int				i;
	char			*word;
	t_quote_state	quote;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		if (!input[i])
			break ;
		if (input[i] == '|')
		{
			token = create_token(TOKEN_PIPE, ft_strdup("|"), QUOTE_NONE);
			add_token(&tokens, token);
			i++;
		}
		else if (input[i] == '<' || input[i] == '>')
		{
			token = handle_redir(input, &i);
			add_token(&tokens, token);
		}
		else
		{
			word = extract_word(input, &i, &quote);
			if (!word)
			{
				free_tokens(tokens);
				return (NULL); // Unclosed quote error
			}
			token = create_token(TOKEN_WORD, word, quote);
			add_token(&tokens, token);
		}
	}
	return (tokens);
}