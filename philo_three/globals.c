/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 22:28:09 by handrow           #+#    #+#             */
/*   Updated: 2021/02/16 01:11:48 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

struct s_settings	g_settings;
t_atomic_bool		g_print_guard;
t_time_ms			g_worker_start;
sem_t				*g_fork_pool;
pid_t				g_philo_pool[MAX_PHILO_NUM];
t_atomic_time_ms	g_deadline;
