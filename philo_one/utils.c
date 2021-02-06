/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 17:04:31 by handrow           #+#    #+#             */
/*   Updated: 2021/02/04 17:05:04 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>

static inline int		ft_isspace(int c)
{
	return (c == '\n' || c == '\r' || c == '\v' || c == '\f' ||
			c == ' ' || c == '\t');
}

unsigned long			ft_atoi(const char *str)
{
	unsigned long	num;

	num = 0;
	while (ft_isspace(*str))
		str++;
	while (*str >= '0' && *str <= '9')
	{
		num = 10 * num + *str - '0';
		str++;
	}
	return (num);
}
