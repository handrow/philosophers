/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 18:26:27 by handrow           #+#    #+#             */
/*   Updated: 2021/02/07 20:42:53 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "philo_one.h"
#include "utils.h"

struct s_settings	g_settings;
t_atomic_bool		g_print_guard;
t_time_ms			g_worker_start;
pthread_mutex_t		g_fork_pool[MAX_PHILO_NUM];
pthread_t			g_philo_pool[MAX_PHILO_NUM];
t_atomic_time_ms	g_deadline_pool[MAX_PHILO_NUM];
t_atomic_bool		g_exited_pool[MAX_PHILO_NUM];

t_time_ms			get_current_time_ms(void)
{
	struct timeval	tv;
	
	gettimeofday(&tv, NULL);
	return ((t_time_ms)tv.tv_sec * 1000LL + (t_time_ms)tv.tv_usec / 1000LL); 
}

void				sleep_until_ms(t_time_ms deadline)
{
	while (true)
	{
		if (get_current_time_ms() >= deadline)
			break;
		usleep(SLEEP_DELAY_US);
	}
}

bool				fill_settings(struct s_settings *settings, int ac, char *av[])
{
	if (ac < PHILO_ARG_LIMIT_MIN || ac > PHILO_ARG_LIMIT_MAX)
		return (false);
	settings->philo_num = ft_atoi(av[PHILO_NUM_ARG_IDX]);
	settings->time_to_die = ft_atoi(av[PHILO_TTD_ARG_IDX]);
	settings->time_to_eat = ft_atoi(av[PHILO_TTE_ARG_IDX]);
	settings->time_to_sleep = ft_atoi(av[PHILO_TTS_ARG_IDX]);
	if (ac == PHILO_ARG_LIMIT_MAX)
		settings->meal_count = ft_atoi(av[PHILO_MEAL_COUNT_ARG_IDX]);
	return (true);
}

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

void				init_atomic_print(t_atomic_bool *print_guard)
{
	print_guard->value = true;
	pthread_mutex_init(&print_guard->guard, NULL);
}

void				destroy_atomic_print(t_atomic_bool *print_guard)
{
	pthread_mutex_destroy(&print_guard->guard);
}

void				print_atomic_message(int philo_idx, const char *msg)
{
	pthread_mutex_lock(&g_print_guard.guard);
	if (g_print_guard.value)
		printf("%6llu ms | %3d | %s\n", get_current_time_ms() - g_worker_start, philo_idx + 1, msg);
	pthread_mutex_unlock(&g_print_guard.guard);
}

void				print_last_atomic_message(int philo_idx, const char *msg)
{
	pthread_mutex_lock(&g_print_guard.guard);
	if (g_print_guard.value)
		printf("%6llu ms | %3d | %s\n", get_current_time_ms() - g_worker_start, philo_idx + 1, msg);
	g_print_guard.value = false;
	pthread_mutex_unlock(&g_print_guard.guard);
}

void				choose_forks(int philo_idx, pthread_mutex_t **ffork, pthread_mutex_t **sfork)
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

void				take_forks(int philo, pthread_mutex_t *ff, pthread_mutex_t *sf)
{
	pthread_mutex_lock(ff);
	print_atomic_message(philo, "has taken a fork");
	pthread_mutex_lock(sf);
	print_atomic_message(philo, "has taken a fork");
}

void				put_forks(pthread_mutex_t *ff, pthread_mutex_t *sf)
{
	pthread_mutex_unlock(ff);
	pthread_mutex_unlock(sf);
}

void				*philo_fedup_handler(int philo_idx)
{
	print_atomic_message(philo_idx, "is full of spagetti");
	set_atomic_time_ms(&g_deadline_pool[philo_idx], get_current_time_ms() + 10000);
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
		set_atomic_time_ms(&g_deadline_pool[philo_idx], get_current_time_ms() + g_settings.time_to_die + 1);
		print_atomic_message(philo_idx, "is sleeping");
		sleep_until_ms(get_current_time_ms() + g_settings.time_to_sleep);
	}
	return (NULL);
}

void				launch_workers(pthread_t philo_pool[], int num)
{
	int	i;

	i = -1;
	while (++i < num)
		pthread_create(&philo_pool[i], NULL, philo_worker, (void*)(size_t)i);
}

void				philo_death_handler(int philo_idx, t_atomic_bool exit_pool[], int num)
{
	int	i;

	print_last_atomic_message(philo_idx, "died");
	i = -1;
	while (++i < num)
		set_atomic_bool(&exit_pool[i], true);
	
}

void				watch_workers(pthread_t philo_pool[], t_atomic_time_ms deadline_pool[],
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
				break;
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

int					main(int ac, char *av[])
{
	if (fill_settings(&g_settings, ac, av) == false)
	{
		printf("Invalid arguments\n");
		return (EXIT_FAILURE);
	}
	init_atomic_print(&g_print_guard);
	init_fork_pool(g_fork_pool, g_settings.philo_num);
	init_status_pools(g_deadline_pool, g_exited_pool, g_settings.philo_num);
	launch_workers(g_philo_pool, g_settings.philo_num);
	watch_workers(g_philo_pool, g_deadline_pool, g_exited_pool, g_settings.philo_num);
	destroy_status_pools(g_deadline_pool, g_exited_pool, g_settings.philo_num);
	destroy_fork_pool(g_fork_pool, g_settings.philo_num);
	destroy_atomic_print(&g_print_guard);
	return (EXIT_SUCCESS);
}
