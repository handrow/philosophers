/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/07 20:30:51 by handrow           #+#    #+#             */
/*   Updated: 2021/02/07 20:39:30 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <stdint.h>
# include <pthread.h>

# define SLEEP_DELAY_US 100
# define MAX_PHILO_NUM 1000
# define PHILO_WORKER_START_DELAY_MS 1000
# define WORKER_WATCHER_DELAY_US 100
# define EVEN_WORKER_START_DELAY_US 200

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
	int			philo_num;
	int			meal_count;
	t_time_ms	time_to_eat;
	t_time_ms	time_to_sleep;
	t_time_ms	time_to_die;
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

extern struct s_settings	g_settings;
extern t_atomic_bool		g_print_guard;
extern t_time_ms			g_worker_start;
extern pthread_mutex_t		g_fork_pool[MAX_PHILO_NUM];
extern pthread_t			g_philo_pool[MAX_PHILO_NUM];
extern t_atomic_time_ms		g_deadline_pool[MAX_PHILO_NUM];
extern t_atomic_bool		g_exited_pool[MAX_PHILO_NUM];

#endif
