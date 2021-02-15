/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 18:29:22 by handrow           #+#    #+#             */
/*   Updated: 2021/02/16 00:05:59 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>

unsigned long			ft_atoi(const char *str);
int						ft_strlen(const char *str);
void					*ft_memcpy(void *dest, const void *src, size_t n);

#endif
