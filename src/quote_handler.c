#include "minishell.h"

int	check_unclosed_quotes(char *input)
{
	int		i;
	char	quote;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			quote = input[i];
			i++;
			while (input[i] && input[i] != quote)
				i++;
			if (!input[i])
				return (0);
			i++;
		}
		else
			i++;
	}
	return (1);
}

char	*remove_quotes(char *str, t_quote_state quote_state)
{
	char	*result;
	int		i;
	int		j;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		// Skip quotes based on quote_state
		if (quote_state == QUOTE_SINGLE && str[i] == '\'')
		{
			i++;
			continue;
		}
		if (quote_state == QUOTE_DOUBLE && str[i] == '"')
		{
			i++;
			continue;
		}
		// For QUOTE_NONE, remove both single and double quotes
		if (quote_state == QUOTE_NONE)
		{
			if (str[i] == '\'' || str[i] == '"')
			{
				i++;
				continue;
			}
		}
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}