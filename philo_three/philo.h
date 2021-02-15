/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/07 20:30:51 by handrow           #+#    #+#             */
/*   Updated: 2021/02/16 01:17:13 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdbool.h>
# include <stdint.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <semaphore.h>

# define SLEEP_DELAY_US 200
# define MAX_PHILO_NUM 200
# define PHILO_WORKER_START_DELAY_MS 1000
# define WORKER_WATCHER_DELAY_US 100

# ifndef FILENAME_MAX
#  define FILENAME_MAX 256
# endif

# define SEMNAME_FORK_POOL "three_forks"
# define SEMNAME_PRINTER "three_printer"
# define SEMNAME_DEADLINE "three_deadline"

enum						e_setting_idx
{
	PHILO_NUM_ARG_IDX = 1,
	PHILO_TTD_ARG_IDX = 2,
	PHILO_TTE_ARG_IDX = 3,
	PHILO_TTS_ARG_IDX = 4,
	PHILO_MEAL_COUNT_ARG_IDX = 5,

	PHILO_ARG_LIMIT_MIN = 5,
	PHILO_ARG_LIMIT_MAX = 6
};

enum						e_exit_status
{
	STATUS_FEDUP,
	STATUS_DIED,
	STATUS_UNKOWN
};

typedef int64_t				t_time_ms;

struct						s_settings
{
	int						philo_num;
	int						meal_count;
	t_time_ms				time_to_eat;
	t_time_ms				time_to_sleep;
	t_time_ms				time_to_die;
};

typedef struct				s_atomic_time_ms
{
	t_time_ms				value;
	sem_t					*guard;
}							t_atomic_time_ms;

typedef struct				s_atomic_bool
{
	bool					value;
	sem_t					*guard;
}							t_atomic_bool;

extern struct s_settings	g_settings;
extern t_atomic_bool		g_print_guard;
extern t_time_ms			g_worker_start;
extern sem_t				*g_fork_pool;
extern pid_t				g_philo_pool[MAX_PHILO_NUM];
extern t_atomic_time_ms		g_deadline;

void						philo_worker(int philo_idx);

void						init_fork_pool(sem_t **forks, int num);

void						destroy_fork_pool(sem_t **forks, int num);

void						destroy_status_pools(int num);

t_time_ms					get_current_time_ms(void);

void						sleep_until_ms(t_time_ms deadline);

void						init_atomic_print(t_atomic_bool *print_guard);

void						destroy_atomic_print(t_atomic_bool *print_guard);

void						print_atomic_message(int idx, const char *msg);

void						print_last_atomic_message(int idx, const char *msg);

bool						get_atomic_bool(t_atomic_bool *atom);

void						set_atomic_bool(t_atomic_bool *atom, bool value);

t_time_ms					get_atomic_time_ms(t_atomic_time_ms *atom);

void						set_atomic_time_ms(t_atomic_time_ms *atom,
												t_time_ms value);

sem_t						*sem_create(const char base_name[], int id,
												int value);

void						sem_delete(const char base_name[], int id);

void						launch_workers(pid_t philo_pool[], int num);

void						watch_workers(pid_t philo_pool[], int num);

void						kill_workers(pid_t philo_pool[], int num);

#endif
