#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>

typedef enum e_token_type
{
	TOKEN_WORD,			// Regular word/command
	TOKEN_PIPE,			// |
	TOKEN_REDIR_IN,		// <
	TOKEN_REDIR_OUT,	// >
	TOKEN_REDIR_APPEND,	// >>
	TOKEN_REDIR_HEREDOC,// <<
	TOKEN_ENV_VAR,		// $VAR
	TOKEN_EXIT_STATUS,	// $?
}	t_token_type;

typedef enum e_quote_state
{
	QUOTE_NONE,
	QUOTE_SINGLE,		// '
	QUOTE_DOUBLE,		// "
}	t_quote_state;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	t_quote_state	quote_state;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	char	**envp;
	t_cmd	*cmds;
	int		last_exit_status;
}	t_shell;

t_token		*lexer(char *input);
t_token		*create_token(t_token_type type, char *value, t_quote_state quote);
void		add_token(t_token **head, t_token *new_token);
void		free_tokens(t_token *tokens);

t_cmd		*parser(t_token *tokens, t_shell *shell);
t_cmd		*create_cmd(void);
void		add_cmd(t_cmd **head, t_cmd *new_cmd);
void		free_cmds(t_cmd *cmds);

void		expand_tokens(t_token *tokens, t_shell *shell);
char		*expand_variables(char *str, t_shell *shell, t_quote_state quote);

char		*remove_quotes(char *str, t_quote_state quote_state);
int			check_unclosed_quotes(char *input);

int			validate_syntax(t_token *tokens);

char		*ft_strdup(const char *s);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
int			ft_strlen(const char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_strcmp(const char *s1, const char *s2);
char		**ft_split(char const *s, char c);
void		free_array(char **arr);
int			ft_isspace(char c);
int			ft_isalnum(char c);

char		*get_env_value(char *var_name, t_shell *shell);
char		*get_prompt(t_shell *shell);

void		print_error(char *msg);
void		syntax_error(char *token);

// Global signal variable (only stores signal number)
volatile int	g_signal;

// Signal handling
void		setup_signals(void);
void		handle_sigint(int sig);
void		handle_sigquit(int sig);

// Environment management
char		**copy_envp(char **envp);
void		free_envp(char **envp);
int			set_env_var(char *var, char *value, t_shell *shell);
int			unset_env_var(char *var, t_shell *shell);
char		**get_envp_array(t_shell *shell);

// Built-in commands
int			ft_echo(char **args);
int			ft_cd(char **args, t_shell *shell);
int			ft_pwd(void);
int			ft_export(char **args, t_shell *shell);
int			ft_unset(char **args, t_shell *shell);
int			ft_env(t_shell *shell);
int			ft_exit(char **args, t_shell *shell);
int			is_builtin(char *cmd);
int			execute_builtin(char **args, t_shell *shell);

// Execution
char		*find_command(char *cmd, t_shell *shell);
int			execute_command(t_cmd *cmd, t_shell *shell);
int			execute_pipeline(t_cmd *cmds, t_shell *shell);
int			setup_redirections(t_redir *redirs, t_shell *shell);
int			handle_heredoc(char *delimiter, t_shell *shell);

#endif