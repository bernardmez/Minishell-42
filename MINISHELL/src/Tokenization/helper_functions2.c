/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmazraan <bmazraan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 23:50:00 by jeid              #+#    #+#             */
/*   Updated: 2026/02/09 16:05:27 by bmazraan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	copy_flag(t_cmd **cmd, int i, char *prompt, t_env *env)
{
	char	*flag;
	int		len;

	flag = skip_to_c(prompt, ' ', env);
	len = flag - prompt;
	env->flag = TRUE;
	flag = dequotencpy(i, len, prompt, &env);
	env->flag = FALSE;
	struct_update_flags(cmd, flag, (*cmd)->flag);
	return (len);
}
