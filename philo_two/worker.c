/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 22:34:04 by handrow           #+#    #+#             */
/*   Updated: 2021/02/16 01:41:38 by handrow          ###   ########.fr       */
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
	int				meal_count;

	meal_count = 0;
	sleep_until_ms(g_worker_start);
	while (!get_atomic_bool(&g_exited_pool[philo_idx]))
	{
		print_atomic_message(philo_idx, "is thinking");
		take_forks(philo_idx);
		print_atomic_message(philo_idx, "is eating");
		sleep_until_ms(get_current_time_ms() + g_settings.time_to_eat);
		put_forks();
		if (g_settings.meal_count > 0 && ++meal_count >= g_settings.meal_count)
			return (philo_fedup_handler(philo_idx));
		set_atomic_time_ms(&g_deadline_pool[philo_idx], get_current_time_ms() + g_settings.time_to_die + 1);
		print_atomic_message(philo_idx, "is sleeping");
		sleep_until_ms(get_current_time_ms() + g_settings.time_to_sleep);
	}
	return (NULL);
}
