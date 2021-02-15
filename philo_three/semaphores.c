/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphores.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 23:37:18 by handrow           #+#    #+#             */
/*   Updated: 2021/02/16 00:05:46 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "philo.h"
#include "utils.h"

static void	sem_name(const char base_name[], int pid, char buff[])
{
	const int	base_name_len = ft_strlen(base_name);
	int			idx;

	idx = 0;
	memcpy(buff, base_name, base_name_len);
	idx = base_name_len;
	buff[idx++] = '_';
	buff[idx++] = (pid / 1000) % 10 + '0';
	buff[idx++] = (pid / 100) % 10 + '0';
	buff[idx++] = (pid / 10) % 10 + '0';
	buff[idx++] = pid % 10 + '0';
	buff[idx++] = '.';
	buff[idx++] = 's';
	buff[idx++] = 'e';
	buff[idx++] = 'm';
	buff[idx++] = '\0';
}

sem_t		*sem_create(const char base_name[], int id, int value)
{
	char sem_name_buff[FILENAME_MAX];

	sem_name(base_name, id, sem_name_buff);
	sem_unlink(sem_name_buff);
	return (sem_open(sem_name_buff, O_CREAT, 0744, value));
}

void		sem_delete(const char base_name[], int id)
{
	char sem_name_buff[FILENAME_MAX];

	sem_name(base_name, id, sem_name_buff);
	sem_unlink(sem_name_buff);
}
