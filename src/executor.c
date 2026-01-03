#include "minishell.h"
#include <sys/stat.h>

static int is_absolute_path(char *cmd)
{
	return (cmd && cmd[0] == '/');
}

static int is_relative_path(char *cmd)
{
	return (cmd && (cmd[0] == '.' || strchr(cmd, '/')));
}

static int is_directory(char *path)
{
	struct stat st;

	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		return (1);
	return (0);
}

char *find_command(char *cmd, t_shell *shell)
{
	char *path;
	char **paths;
	int i;
	char *temp;

	if (!cmd || !cmd[0])
		return (NULL);
	if (is_absolute_path(cmd) || is_relative_path(cmd))
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path = get_env_value("PATH", shell);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (temp)
		{
			char *full_path;
			full_path = ft_strjoin(temp, cmd);
			free(temp);
			if (full_path && access(full_path, F_OK) == 0)
			{
				free_array(paths);
				return (full_path);
			}
			if (full_path)
				free(full_path);
		}
		i++;
	}
	free_array(paths);
	return (NULL);
}

static void restore_redirections(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

int handle_heredoc(char *delimiter, t_shell *shell)
{
	char *line;
	int pipe_fd[2];
	int delim_len;
	char buffer[4096];
	int is_tty;

	(void)shell;
	if (pipe(pipe_fd) == -1)
		return (-1);
	delim_len = ft_strlen(delimiter);
	is_tty = isatty(STDIN_FILENO);
	while (1)
	{
		if (is_tty)
		{
			line = readline("> ");
			if (!line)
				break;
		}
		else
		{
			if (fgets(buffer, sizeof(buffer), stdin) == NULL)
				break;
			line = ft_strdup(buffer);
			if (line && ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = '\0';
		}
		if (!line)
			break;
		if (ft_strncmp(line, delimiter, delim_len) == 0 && line[delim_len] == '\0')
		{
			free(line);
			break;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

int setup_redirections(t_redir *redirs, t_shell *shell)
{
	t_redir *current;
	int fd;
	int heredoc_fd;

	current = redirs;
	while (current)
	{
		if (current->type == TOKEN_REDIR_IN)
		{
			fd = open(current->file, O_RDONLY);
			if (fd == -1)
			{
				write(2, "minishell: ", 11);
				write(2, current->file, ft_strlen(current->file));
				write(2, ": ", 2);
				write(2, strerror(errno), ft_strlen(strerror(errno)));
				write(2, "\n", 1);
				return (0);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (current->type == TOKEN_REDIR_OUT)
		{
			fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				write(2, "minishell: ", 11);
				write(2, current->file, ft_strlen(current->file));
				write(2, ": ", 2);
				write(2, strerror(errno), ft_strlen(strerror(errno)));
				write(2, "\n", 1);
				return (0);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (current->type == TOKEN_REDIR_APPEND)
		{
			fd = open(current->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				write(2, "minishell: ", 11);
				write(2, current->file, ft_strlen(current->file));
				write(2, ": ", 2);
				write(2, strerror(errno), ft_strlen(strerror(errno)));
				write(2, "\n", 1);
				return (0);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (current->type == TOKEN_REDIR_HEREDOC)
		{
			heredoc_fd = handle_heredoc(current->file, shell);
			if (heredoc_fd == -1)
				return (0);
			dup2(heredoc_fd, STDIN_FILENO);
			close(heredoc_fd);
		}
		current = current->next;
	}
	return (1);
}

int execute_command(t_cmd *cmd, t_shell *shell)
{
	char *cmd_path;
	int saved_stdin;
	int saved_stdout;
	int ret;
	pid_t pid;
	int status;

	if (!cmd || !cmd->args)
		return (0);
	// If no command but there are redirections, handle them and return 0
	if (!cmd->args[0] && cmd->redirs)
	{
		int saved_stdin = dup(STDIN_FILENO);
		int saved_stdout = dup(STDOUT_FILENO);
		if (!setup_redirections(cmd->redirs, shell))
		{
			restore_redirections(saved_stdin, saved_stdout);
			return (1);
		}
		restore_redirections(saved_stdin, saved_stdout);
		return (0);
	}
	if (!cmd->args[0])
		return (0);
	if (is_builtin(cmd->args[0]))
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		if (!setup_redirections(cmd->redirs, shell))
		{
			restore_redirections(saved_stdin, saved_stdout);
			return (1);
		}
		ret = execute_builtin(cmd->args, shell);
		restore_redirections(saved_stdin, saved_stdout);
		return (ret);
	}
	cmd_path = find_command(cmd->args[0], shell);
	if (!cmd_path)
	{
		write(2, "minishell: ", 11);
		write(2, cmd->args[0], ft_strlen(cmd->args[0]));
		write(2, ": command not found\n", 20);
		return (127);
	}
	// Check if it's a directory
	if (is_directory(cmd_path))
	{
		write(2, "minishell: ", 11);
		write(2, cmd_path, ft_strlen(cmd_path));
		write(2, ": Is a directory\n", 18);
		free(cmd_path);
		return (126);
	}
	pid = fork();
	if (pid == 0)
	{
		if (!setup_redirections(cmd->redirs, shell))
			exit(1);
		execve(cmd_path, cmd->args, get_envp_array(shell));
		write(2, "minishell: ", 11);
		write(2, cmd_path, ft_strlen(cmd_path));
		write(2, ": ", 2);
		write(2, strerror(errno), ft_strlen(strerror(errno)));
		write(2, "\n", 1);
		free(cmd_path);
		exit(126);
	}
	free(cmd_path);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

static int count_commands(t_cmd *cmds)
{
	int count;

	count = 0;
	while (cmds)
	{
		count++;
		cmds = cmds->next;
	}
	return (count);
}

int execute_pipeline(t_cmd *cmds, t_shell *shell)
{
	int num_cmds;
	int *pipes;
	int i;
	int j;
	pid_t *pid;
	int status;
	int ret;
	char *cmd_path;
	t_cmd *current;

	if (!cmds)
		return (0);
	num_cmds = count_commands(cmds);
	if (num_cmds == 1 && is_builtin(cmds->args[0]))
		return (execute_command(cmds, shell));
	pipes = malloc(sizeof(int) * (num_cmds - 1) * 2);
	pid = malloc(sizeof(pid_t) * num_cmds);
	if (!pipes || !pid)
		return (1);
	i = 0;
	while (i < num_cmds - 1)
	{
		if (pipe(pipes + i * 2) == -1)
		{
			free(pipes);
			free(pid);
			return (1);
		}
		i++;
	}
	i = 0;
	current = cmds;
	while (current && i < num_cmds)
	{
		pid[i] = fork();
		if (pid[i] == 0)
		{
			if (i > 0)
				dup2(pipes[(i - 1) * 2], STDIN_FILENO);
			if (i < num_cmds - 1)
				dup2(pipes[i * 2 + 1], STDOUT_FILENO);
			j = 0;
			while (j < (num_cmds - 1) * 2)
			{
				close(pipes[j]);
				j++;
			}
			if (!setup_redirections(current->redirs, shell))
				exit(1);
			// Handle empty command (no args but may have redirections)
			if (!current->args || !current->args[0])
			{
				exit(0);
			}
			if (is_builtin(current->args[0]))
				exit(execute_builtin(current->args, shell));
			cmd_path = find_command(current->args[0], shell);
			if (!cmd_path)
			{
				write(2, "minishell: ", 11);
				write(2, current->args[0], ft_strlen(current->args[0]));
				write(2, ": command not found\n", 20);
				exit(127);
			}
			// Check if it's a directory
			if (is_directory(cmd_path))
			{
				write(2, "minishell: ", 11);
				write(2, cmd_path, ft_strlen(cmd_path));
				write(2, ": Is a directory\n", 18);
				free(cmd_path);
				exit(126);
			}
			execve(cmd_path, current->args, get_envp_array(shell));
			write(2, "minishell: ", 11);
			write(2, cmd_path, ft_strlen(cmd_path));
			write(2, ": ", 2);
			write(2, strerror(errno), ft_strlen(strerror(errno)));
			write(2, "\n", 1);
			free(cmd_path);
			exit(126);
		}
		i++;
		current = current->next;
	}
	j = 0;
	while (j < (num_cmds - 1) * 2)
	{
		close(pipes[j]);
		j++;
	}
	i = 0;
	ret = 0;
	while (i < num_cmds)
	{
		waitpid(pid[i], &status, 0);
		if (i == num_cmds - 1)
			ret = WEXITSTATUS(status);
		i++;
	}
	free(pipes);
	free(pid);
	return (ret);
}
