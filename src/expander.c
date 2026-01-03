#include "minishell.h"

char	*get_env_value(char *var_name, t_shell *shell)
{
	int		i;
	int		len;

	if (!var_name || !shell || !shell->envp)
		return (NULL);
	len = ft_strlen(var_name);
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], var_name, len) == 0
			&& shell->envp[i][len] == '=')
			return (shell->envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

static char	*extract_var_name(char *str, int *i)
{
	int		start;
	char	*var_name;

	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	if (*i == start)
		return (ft_strdup(""));
	var_name = ft_substr(str, start, *i - start);
	return (var_name);
}

static char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	int		tmp;
	int		neg;

	neg = (n < 0);
	tmp = n;
	len = (neg ? 1 : 0);
	while (tmp && ++len)
		tmp /= 10;
	len = (len == 0) ? 1 : len;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (neg)
		str[0] = '-';
	if (n == 0)
		str[0] = '0';
	while (n)
	{
		str[--len] = (neg ? -(n % 10) : (n % 10)) + '0';
		n /= 10;
	}
	return (str);
}

char	*expand_variables(char *str, t_shell *shell, t_quote_state quote)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	char	*temp;
	int		i;
	int		start;

	if (!str || quote == QUOTE_SINGLE)
		return (ft_strdup(str));
	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		start = i;
		while (str[i] && str[i] != '$')
			i++;
		if (i > start)
		{
			temp = ft_substr(str, start, i - start);
			result = ft_strjoin(result, temp);
			free(temp);
		}
		if (str[i] == '$')
		{
			var_name = extract_var_name(str, &i);
			// If var_name is empty (just $ with nothing after), keep the $
			if (ft_strlen(var_name) == 0)
			{
				temp = result;
				result = ft_strjoin(result, "$");
				free(temp);
				free(var_name);
			}
			else if (ft_strncmp(var_name, "?", 2) == 0)
			{
				var_value = ft_itoa(shell->last_exit_status);
				temp = result;
				result = ft_strjoin(result, var_value);
				free(temp);
				free(var_value);
				free(var_name);
			}
			else
			{
				var_value = get_env_value(var_name, shell);
				if (var_value)
				{
					temp = result;
					result = ft_strjoin(result, var_value);
					free(temp);
				}
				free(var_name);
			}
		}
	}
	return (result);
}

void	expand_tokens(t_token *tokens, t_shell *shell)
{
	t_token	*current;
	char	*expanded;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			expanded = expand_variables(current->value, shell, 
				current->quote_state);
			free(current->value);
			current->value = expanded;
		}
		current = current->next;
	}
}