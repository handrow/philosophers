/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 18:26:27 by handrow           #+#    #+#             */
/*   Updated: 2021/02/16 16:27:23 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
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

static void	launch_workers(pthread_t philo_pool[], int num)
{
	int	i;

	i = -1;
	while (++i < num)
		pthread_create(&philo_pool[i], NULL, philo_worker, (void*)(size_t)i);
}

static void	philo_death_handler(int philo_idx, t_atomic_bool exit_pool[],
								int num)
{
	int	i;

	print_last_atomic_message(philo_idx, "died");
	i = -1;
	while (++i < num)
		set_atomic_bool(&exit_pool[i], true);
}

static void	watch_workers(pthread_t philo_pool[],
						t_atomic_time_ms deadline_pool[],
						t_atomic_bool exit_pool[], int num)
{
	int		i;
	bool	run;
	bool	sum;

	run = true;
	while (run)
	{
		sum = true;
		i = -1;
		while (++i < num)
		{
			if (get_current_time_ms() >= get_atomic_time_ms(&deadline_pool[i]))
			{
				philo_death_handler(i, exit_pool, num);
				run = false;
				break ;
			}
			sum = sum && get_atomic_bool(&exit_pool[i]);
		}
		run = !sum;
		usleep(WORKER_WATCHER_DELAY_US);
	}
	i = -1;
	while (++i < num)
		pthread_join(philo_pool[i], NULL);
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
	init_status_pools(g_deadline_pool, g_exited_pool, g_settings.philo_num);
	launch_workers(g_philo_pool, g_settings.philo_num);
	watch_workers(g_philo_pool, g_deadline_pool, g_exited_pool,
				g_settings.philo_num);
	destroy_status_pools(g_deadline_pool, g_exited_pool, g_settings.philo_num);
	destroy_fork_pool(&g_fork_pool, g_settings.philo_num);
	destroy_atomic_print(&g_print_guard);
	return (EXIT_SUCCESS);
}
