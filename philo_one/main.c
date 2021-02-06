/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 18:26:27 by handrow           #+#    #+#             */
/*   Updated: 2021/02/06 22:13:32 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "utils.h"

#define SLEEP_DELAY_US 100
#define	MAX_PHILO_NUM 1000
#define PHILO_WORKER_START_DELAY_MS 1000

typedef int64_t		t_time_ms;

enum				e_setting_idx
{
	PHILO_NUM_ARG_IDX = 1,
	PHILO_TTD_ARG_IDX = 2,
	PHILO_TTE_ARG_IDX = 3,
	PHILO_TTS_ARG_IDX = 4,
	PHILO_MEAL_COUNT_ARG_IDX = 5,

	PHILO_ARG_LIMIT_MIN = 5,
	PHILO_ARG_LIMIT_MAX = 6
};

struct				s_settings
{
	int				philo_num;
	int				meal_count;
	t_time_ms	 	time_to_eat;
	t_time_ms	 	time_to_sleep;
	t_time_ms	 	time_to_die;
};

typedef struct		s_atomic_time_ms
{
	t_time_ms		value;
	pthread_mutex_t	guard;
}					t_atomic_time_ms;

typedef struct		s_atomic_bool
{
	bool			value;
	pthread_mutex_t	guard;
}					t_atomic_bool;

struct s_settings	g_settings;
pthread_mutex_t		g_print_guard;
t_time_ms			g_worker_start;
pthread_mutex_t		g_fork_pool[MAX_PHILO_NUM];
pthread_t			g_philo_pool[MAX_PHILO_NUM];
t_atomic_time_ms	g_deadline_pool[MAX_PHILO_NUM];
t_atomic_bool		g_exited_pool[MAX_PHILO_NUM];

bool				get_atomic_bool(t_atomic_bool *atom)
{
	bool	val;

	pthread_mutex_lock(&atom->guard);
	val = atom->value;
	pthread_mutex_unlock(&atom->guard);
	return (val);
}

void				set_atomic_bool(t_atomic_bool *atom, bool value)
{
	pthread_mutex_lock(&atom->guard);
	atom->value = value;
	pthread_mutex_unlock(&atom->guard);
}

bool				get_atomic_time_ms(t_atomic_time_ms *atom)
{
	t_time_ms	val;

	pthread_mutex_lock(&atom->guard);
	val = atom->value;
	pthread_mutex_unlock(&atom->guard);
	return (val);
}

void				set_atomic_time_ms(t_atomic_time_ms *atom, bool value)
{
	pthread_mutex_lock(&atom->guard);
	atom->value = value;
	pthread_mutex_unlock(&atom->guard);
}

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
		deadline_pool[i].value = g_worker_start + g_settings.time_to_die;
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

void				init_atomic_print(pthread_mutex_t *print_guard)
{
	pthread_mutex_init(print_guard, NULL);
}

void				destroy_atomic_print(pthread_mutex_t *print_guard)
{
	pthread_mutex_destroy(print_guard);
}

void				print_atomic_message(int philo_idx, const char *msg)
{
	pthread_mutex_lock(&g_print_guard);
	printf("%-6llu ms | %-3d | %s\n", get_current_time_ms(), philo_idx + 1, msg);
	pthread_mutex_unlock(&g_print_guard);
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
	// LAUNCH WORKRES
	// WATCH WORKERS
	destroy_status_pools(g_deadline_pool, g_exited_pool, g_settings.philo_num);
	destroy_fork_pool(g_fork_pool, g_settings.philo_num);
	destroy_atomic_print(&g_print_guard);
	return (EXIT_SUCCESS);
}
