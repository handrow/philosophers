/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 18:26:27 by handrow           #+#    #+#             */
/*   Updated: 2021/02/16 16:29:12 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "philo.h"
#include "utils.h"

static bool	fill_settings(struct s_settings *settings, int ac, char *av[])
{
	if (ac < PHILO_ARG_LIMIT_MIN || ac > PHILO_ARG_LIMIT_MAX)
		return (false);
	settings->philo_num = ft_atoi(av[PHILO_NUM_ARG_IDX]);
	settings->time_to_die = ft_atoi(av[PHILO_TTD_ARG_IDX]);
	settings->time_to_eat = ft_atoi(av[PHILO_TTE_ARG_IDX]);
	settings->time_to_sleep = ft_atoi(av[PHILO_TTS_ARG_IDX]);
	if (ac == PHILO_ARG_LIMIT_MAX
	&& (settings->meal_count = ft_atoi(av[PHILO_MEAL_COUNT_ARG_IDX])) <= 0)
		return (false);
	return (!(
		(settings->philo_num < 2 || settings->philo_num > 200) ||
		(settings->time_to_die < 60) || (settings->time_to_eat < 60) ||
		(settings->time_to_sleep < 60)));
}

int			main(int ac, char *av[])
{
	if (fill_settings(&g_settings, ac, av) == false)
	{
		printf("Invalid arguments\n");
		return (EXIT_FAILURE);
	}
	init_atomic_print(&g_print_guard);
	init_fork_pool(&g_fork_pool, g_settings.philo_num);
	g_worker_start = get_current_time_ms() + PHILO_WORKER_START_DELAY_MS;
	launch_workers(g_philo_pool, g_settings.philo_num);
	watch_workers(g_philo_pool, g_settings.philo_num);
	kill_workers(g_philo_pool, g_settings.philo_num);
	destroy_status_pools(g_settings.philo_num);
	destroy_fork_pool(&g_fork_pool, g_settings.philo_num);
	destroy_atomic_print(&g_print_guard);
	return (EXIT_SUCCESS);
}
