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
	char	quote;

	if (!str || quote_state == QUOTE_NONE)
		return (ft_strdup(str));
	result = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"'))
		{
			quote = str[i];
			i++;
			while (str[i] && str[i] != quote)
				result[j++] = str[i++];
			if (str[i] == quote)
				i++;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}