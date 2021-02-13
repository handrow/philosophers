/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 22:32:46 by handrow           #+#    #+#             */
/*   Updated: 2021/02/13 22:33:20 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void				init_fork_pool(pthread_mutex_t fork_pool[], int num)
{
	int	i;

	i = 0;
	while (i < num)
		pthread_mutex_init(&fork_pool[i++], NULL);
}

void				destroy_fork_pool(pthread_mutex_t fork_pool[], int num)
{
	int	i;

	i = 0;
	while (i < num)
		pthread_mutex_destroy(&fork_pool[i++]);
}

void				init_status_pools(t_atomic_time_ms deadline_pool[],
										t_atomic_bool exit_pool[], int num)
{
	int	i;

	g_worker_start = get_current_time_ms() + PHILO_WORKER_START_DELAY_MS;
	i = 0;
	while (i < num)
	{
		deadline_pool[i].value = g_worker_start + g_settings.time_to_die + 1;
		pthread_mutex_init(&(deadline_pool[i++].guard), NULL);
	}
	i = 0;
	while (i < num)
	{
		exit_pool[i].value = 0;
		pthread_mutex_init(&(exit_pool[i++].guard), NULL);
	}
}

void				destroy_status_pools(t_atomic_time_ms deadline_pool[],
										t_atomic_bool exit_pool[], int num)
{
	int	i;

	i = 0;
	while (i < num)
		pthread_mutex_destroy(&(deadline_pool[i++].guard));
	i = 0;
	while (i < num)
		pthread_mutex_destroy(&(exit_pool[i++].guard));
}
