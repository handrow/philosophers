/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 22:34:04 by handrow           #+#    #+#             */
/*   Updated: 2021/02/16 16:30:08 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void			take_forks(int philo)
{
	sem_wait(g_fork_pool);
	print_atomic_message(philo, "has taken a fork");
	print_atomic_message(philo, "has taken a fork");
}

static void			put_forks(void)
{
	sem_post(g_fork_pool);
}

static void			philo_fedup_handler(int philo_idx)
{
	set_atomic_time_ms(&g_deadline, MAX_TIME_MS);
	print_atomic_message(philo_idx, "is full of spagetti");
	sem_close(g_deadline.guard);
	sem_delete(SEMNAME_DEADLINE, philo_idx);
	exit(STATUS_FEDUP);
}

static void			*philo_deadline_checker(void *param)
{
	const int	philo_idx = (int)(size_t)param;

	while (true)
	{
		if (get_current_time_ms() >= get_atomic_time_ms(&g_deadline))
			break ;
		usleep(WORKER_WATCHER_DELAY_US);
	}
	print_last_atomic_message(philo_idx, "died");
	sem_close(g_deadline.guard);
	sem_delete(SEMNAME_DEADLINE, philo_idx);
	exit(STATUS_DIED);
	return (NULL);
}

void				philo_worker(int philo_idx)
{
	pthread_t		watcher;
	int				meal_count;

	meal_count = 0;
	pthread_create(&watcher, NULL,
					philo_deadline_checker, (void *)(size_t)philo_idx);
	pthread_detach(watcher);
	sleep_until_ms(g_worker_start);
	while (true)
	{
		print_atomic_message(philo_idx, "is thinking");
		take_forks(philo_idx);
		print_atomic_message(philo_idx, "is eating");
		sleep_until_ms(get_current_time_ms() + g_settings.time_to_eat);
		put_forks();
		if (g_settings.meal_count > 0 && ++meal_count >= g_settings.meal_count)
			philo_fedup_handler(philo_idx);
		set_atomic_time_ms(&g_deadline,
			get_current_time_ms() + g_settings.time_to_die + 1);
		print_atomic_message(philo_idx, "is sleeping");
		sleep_until_ms(get_current_time_ms() + g_settings.time_to_sleep);
	}
}
