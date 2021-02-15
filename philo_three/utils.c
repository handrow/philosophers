/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 17:04:31 by handrow           #+#    #+#             */
/*   Updated: 2021/02/16 00:06:05 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stddef.h>

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

int						ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void					*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = dest;
	s = src;
	if (dest == src)
		return (dest);
	while (n--)
		*d++ = *s++;
	return (dest);
}
