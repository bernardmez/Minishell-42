#include "minishell.h"

void	print_error(char *msg)
{
	write(2, "minishell: ", 11);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

void	syntax_error(char *token)
{
	write(2, "minishell: syntax error near unexpected token `", 48);
	write(2, token, ft_strlen(token));
	write(2, "'\n", 2);
}