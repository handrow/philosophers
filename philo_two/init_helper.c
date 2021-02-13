/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 22:32:46 by handrow           #+#    #+#             */
/*   Updated: 2021/02/14 01:21:15 by handrow          ###   ########.fr       */
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

void				init_status_pools(t_atomic_time_ms deadline_pool[],
										t_atomic_bool exit_pool[], int num)
{
	int	i;

	g_worker_start = get_current_time_ms() + PHILO_WORKER_START_DELAY_MS;
	i = -1;
	while (++i < num)
	{
		deadline_pool[i].value = g_worker_start + g_settings.time_to_die + 1;
		deadline_pool[i].guard = sem_create(SEMNAME_DEADLINE, i, 1);
	}
	i = -1;
	while (++i < num)
	{
		exit_pool[i].value = 0;
		exit_pool[i].guard = sem_create(SEMNAME_EXITED, i, 1);
	}
}

void				destroy_status_pools(t_atomic_time_ms deadline_pool[],
										t_atomic_bool exit_pool[], int num)
{
	int	i;

	i = -1;
	while (++i < num)
	{
		sem_close(deadline_pool[i].guard);
		sem_delete(SEMNAME_DEADLINE, i);
	}
	i = -1;
	while (++i < num)
	{
		sem_close(exit_pool[i].guard);
		sem_delete(SEMNAME_EXITED, i);
	}
}
