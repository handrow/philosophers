/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_status.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 01:14:41 by handrow           #+#    #+#             */
/*   Updated: 2021/02/16 01:16:49 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "philo.h"

void				launch_workers(pid_t philo_pool[], int num)
{
	int	i;

	i = -1;
	while (++i < num)
	{
		philo_pool[i] = fork();
		if (philo_pool[i] == 0)
		{
			g_deadline.value = g_worker_start + g_settings.time_to_die + 1;
			g_deadline.guard = sem_create(SEMNAME_DEADLINE, i, 1);
			philo_worker(i);
		}
	}
}

enum e_exit_status	philo_check_exit_status(pid_t p)
{
	int stat;

	if (waitpid(p, &stat, WNOHANG) > 0)
	{
		if (WIFEXITED(stat))
			return (WEXITSTATUS(stat) == STATUS_FEDUP ? STATUS_FEDUP : STATUS_DIED);
	}
	return (STATUS_UNKOWN);
}

void				watch_workers(pid_t philo_pool[], int num)
{
	int					i;
	enum e_exit_status	philo_status;
	bool				feduped[MAX_PHILO_NUM];
	bool				stop;

	stop = false;
	memset(feduped, false, sizeof(feduped));
	while (!stop)
	{
		i = -1;
		stop = true;
		while (++i < num)
		{
			if (feduped[i] != true)
			{
				philo_status = philo_check_exit_status(philo_pool[i]);
				if (philo_status == STATUS_DIED)
					return ;
				else if (philo_status == STATUS_FEDUP)
					feduped[i] = true;
			}
			stop = stop && feduped[i];
		}
		usleep(WORKER_WATCHER_DELAY_US);
	}
}

void				kill_workers(pid_t philo_pool[], int num)
{
	int		i;
	int		stat;

	i = -1;
	while (++i < num)
		kill(philo_pool[i], SIGKILL);
	while (wait(&stat) > 0)
		;
}
