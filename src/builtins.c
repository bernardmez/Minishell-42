#include "minishell.h"

int	ft_echo(char **args)
{
	int	n_flag;
	int	i;

	n_flag = 0;
	i = 1;
	if (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_flag)
		write(1, "\n", 1);
	return (0);
}

int	ft_cd(char **args, t_shell *shell)
{
	char	*path;
	char	*old_pwd;
	char	*cwd;

	if (!args[1])
	{
		path = get_env_value("HOME", shell);
		if (!path)
		{
			print_error("cd: HOME not set");
			return (1);
		}
	}
	else
		path = args[1];
	old_pwd = getcwd(NULL, 0);
	if (chdir(path) != 0)
	{
		print_error("cd: No such file or directory");
		free(old_pwd);
		return (1);
	}
	cwd = getcwd(NULL, 0);
	if (old_pwd)
		set_env_var("OLDPWD", old_pwd, shell);
	if (cwd)
		set_env_var("PWD", cwd, shell);
	free(old_pwd);
	free(cwd);
	return (0);
}

int	ft_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		print_error("pwd: error retrieving current directory");
		return (1);
	}
	write(1, cwd, ft_strlen(cwd));
	write(1, "\n", 1);
	free(cwd);
	return (0);
}

static int	is_valid_var_name(char *var)
{
	int	i;

	if (!var || !var[0])
		return (0);
	if (!((var[0] >= 'a' && var[0] <= 'z')
		|| (var[0] >= 'A' && var[0] <= 'Z') || var[0] == '_'))
		return (0);
	i = 1;
	while (var[i] && var[i] != '=')
	{
		if (!((var[i] >= 'a' && var[i] <= 'z')
			|| (var[i] >= 'A' && var[i] <= 'Z')
			|| (var[i] >= '0' && var[i] <= '9') || var[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	ft_export(char **args, t_shell *shell)
{
	int		i;
	char	*var_name;
	char	*value;
	char	*equal_pos;

	if (!args[1])
	{
		ft_env(shell);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (!is_valid_var_name(args[i]))
		{
			write(2, "minishell: export: `", 20);
			write(2, args[i], ft_strlen(args[i]));
			write(2, "': not a valid identifier\n", 27);
			return (1);
		}
		equal_pos = strchr(args[i], '=');
		if (equal_pos)
		{
			var_name = ft_substr(args[i], 0, equal_pos - args[i]);
			if (var_name)
			{
				value = ft_strdup(equal_pos + 1);
				if (value)
				{
					set_env_var(var_name, value, shell);
					free(value);
				}
				free(var_name);
			}
		}
		i++;
	}
	return (0);
}

int	ft_unset(char **args, t_shell *shell)
{
	int	i;
	int	ret;

	if (!args[1])
		return (0);
	ret = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_var_name(args[i]))
		{
			write(2, "minishell: unset: `", 19);
			write(2, args[i], ft_strlen(args[i]));
			write(2, "': not a valid identifier\n", 26);
			ret = 1;
		}
		else
			unset_env_var(args[i], shell);
		i++;
	}
	return (ret);
}

int	ft_env(t_shell *shell)
{
	int	i;

	if (!shell || !shell->envp)
		return (1);
	i = 0;
	while (shell->envp[i])
	{
		write(1, shell->envp[i], ft_strlen(shell->envp[i]));
		write(1, "\n", 1);
		i++;
	}
	return (0);
}

static int	is_numeric(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **args, t_shell *shell)
{
	int	exit_code;

	(void)shell;
	write(1, "exit\n", 5);
	if (!args[1])
		exit(0);
	if (!is_numeric(args[1]))
	{
		write(2, "minishell: exit: ", 17);
		write(2, args[1], ft_strlen(args[1]));
		write(2, ": numeric argument required\n", 28);
		exit(255);
	}
	exit_code = atoi(args[1]);
	if (args[2])
	{
		print_error("exit: too many arguments");
		return (1);
	}
	exit(exit_code);
}

static int	is_colon_command(char *cmd)
{
	int	i;

	if (!cmd || !cmd[0])
		return (0);
	i = 0;
	while (cmd[i] == ':')
		i++;
	return (cmd[i] == '\0' && i > 0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (is_colon_command(cmd))
		return (1);
	return (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

int	execute_builtin(char **args, t_shell *shell)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return (ft_echo(args));
	if (ft_strcmp(args[0], "cd") == 0)
		return (ft_cd(args, shell));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (ft_pwd());
	if (ft_strcmp(args[0], "export") == 0)
		return (ft_export(args, shell));
	if (ft_strcmp(args[0], "unset") == 0)
		return (ft_unset(args, shell));
	if (ft_strcmp(args[0], "env") == 0)
		return (ft_env(shell));
	if (ft_strcmp(args[0], "exit") == 0)
		return (ft_exit(args, shell));
	if (is_colon_command(args[0]))
		return (0); // :, ::, :::, etc. are no-ops that always succeed
	return (1);
}

