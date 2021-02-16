/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 22:34:04 by handrow           #+#    #+#             */
/*   Updated: 2021/02/16 16:23:45 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void			choose_forks(int philo_idx,
								pthread_mutex_t **ffork,
								pthread_mutex_t **sfork)
{
	if (philo_idx == g_settings.philo_num - 1)
	{
		*ffork = &g_fork_pool[(philo_idx + 1) % g_settings.philo_num];
		*sfork = &g_fork_pool[philo_idx];
	}
	else
	{
		*ffork = &g_fork_pool[philo_idx];
		*sfork = &g_fork_pool[(philo_idx + 1) % g_settings.philo_num];
	}
}

static void			take_forks(int philo,
							pthread_mutex_t *ff,
							pthread_mutex_t *sf)
{
	pthread_mutex_lock(ff);
	print_atomic_message(philo, "has taken a fork");
	pthread_mutex_lock(sf);
	print_atomic_message(philo, "has taken a fork");
}

static void			put_forks(pthread_mutex_t *ff, pthread_mutex_t *sf)
{
	pthread_mutex_unlock(ff);
	pthread_mutex_unlock(sf);
}

static void			*philo_fedup_handler(int philo_idx)
{
	set_atomic_time_ms(&g_deadline_pool[philo_idx], MAX_TIME_MS);
	print_atomic_message(philo_idx, "is full of spagetti");
	set_atomic_bool(&g_exited_pool[philo_idx], true);
	return (NULL);
}

void				*philo_worker(void *param)
{
	const int		philo_idx = (int)(size_t)param;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	int				meal_count;

	meal_count = 0;
	choose_forks(philo_idx, &first_fork, &second_fork);
	sleep_until_ms(g_worker_start);
	if ((philo_idx % 2) == 1)
		usleep(EVEN_WORKER_START_DELAY_US);
	while (!get_atomic_bool(&g_exited_pool[philo_idx]))
	{
		print_atomic_message(philo_idx, "is thinking");
		take_forks(philo_idx, first_fork, second_fork);
		print_atomic_message(philo_idx, "is eating");
		sleep_until_ms(get_current_time_ms() + g_settings.time_to_eat);
		put_forks(first_fork, second_fork);
		if (g_settings.meal_count > 0 && ++meal_count >= g_settings.meal_count)
			return (philo_fedup_handler(philo_idx));
		set_atomic_time_ms(&g_deadline_pool[philo_idx],
					get_current_time_ms() + g_settings.time_to_die + 1);
		print_atomic_message(philo_idx, "is sleeping");
		sleep_until_ms(get_current_time_ms() + g_settings.time_to_sleep);
	}
	return (NULL);
}
