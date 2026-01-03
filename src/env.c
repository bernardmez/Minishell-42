#include "minishell.h"

static int	count_env_vars(char **envp)
{
	int	count;

	count = 0;
	while (envp && envp[count])
		count++;
	return (count);
}

char	**copy_envp(char **envp)
{
	char	**new_envp;
	int		count;
	int		i;

	count = count_env_vars(envp);
	new_envp = malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			free_envp(new_envp);
			return (NULL);
		}
		i++;
	}
	new_envp[count] = NULL;
	return (new_envp);
}

void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

static int	find_env_index(char *var, t_shell *shell)
{
	int	i;
	int	len;

	if (!var || !shell || !shell->envp)
		return (-1);
	len = ft_strlen(var);
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], var, len) == 0
			&& shell->envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	set_env_var(char *var, char *value, t_shell *shell)
{
	int		index;
	char	*new_var;
	char	*temp;
	int		count;
	char	**new_envp;
	int		i;

	if (!var || !shell)
		return (0);
	temp = ft_strjoin(var, "=");
	if (!temp)
		return (0);
	new_var = ft_strjoin(temp, value);
	free(temp);
	if (!new_var)
		return (0);
	index = find_env_index(var, shell);
	if (index >= 0)
	{
		free(shell->envp[index]);
		shell->envp[index] = new_var;
		return (1);
	}
	count = count_env_vars(shell->envp);
	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
	{
		free(new_var);
		return (0);
	}
	i = 0;
	while (i < count)
	{
		new_envp[i] = shell->envp[i];
		i++;
	}
	new_envp[count] = new_var;
	new_envp[count + 1] = NULL;
	free(shell->envp);
	shell->envp = new_envp;
	return (1);
}

int	unset_env_var(char *var, t_shell *shell)
{
	int		index;
	int		count;
	char	**new_envp;
	int		i;
	int		j;

	if (!var || !shell)
		return (0);
	index = find_env_index(var, shell);
	if (index < 0)
		return (0);
	count = count_env_vars(shell->envp);
	new_envp = malloc(sizeof(char *) * count);
	if (!new_envp)
		return (0);
	i = 0;
	j = 0;
	while (i < count)
	{
		if (i != index)
			new_envp[j++] = shell->envp[i];
		else
			free(shell->envp[i]);
		i++;
	}
	new_envp[j] = NULL;
	free(shell->envp);
	shell->envp = new_envp;
	return (1);
}

char	**get_envp_array(t_shell *shell)
{
	return (shell->envp);
}

