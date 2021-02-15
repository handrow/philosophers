/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 22:32:46 by handrow           #+#    #+#             */
/*   Updated: 2021/02/16 01:08:35 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void				init_fork_pool(sem_t **forks, int num)
{
	*forks = sem_create(SEMNAME_FORK_POOL, 0, num / 2);
}

void				destroy_fork_pool(sem_t **forks, int num)
{
	(void)num;
	sem_close(*forks);
	sem_delete(SEMNAME_FORK_POOL, 0);
	*forks = NULL;
}

void				destroy_status_pools(int num)
{
	int	i;

	i = -1;
	while (++i < num)
		sem_delete(SEMNAME_DEADLINE, i);
}
