/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeid <jeid@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 21:30:28 by jeid              #+#    #+#             */
/*   Updated: 2026/01/15 21:30:29 by jeid             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_error(t_env **env, char *errmessage, int error_code, int fork)
{
	printf("%s\n", errmessage);
	(*env)->exit_status = 1;
	if (fork)
		exit(error_code);
	(*env)->exit_code = error_code;
}
