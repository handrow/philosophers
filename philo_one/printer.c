/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 22:30:49 by handrow           #+#    #+#             */
/*   Updated: 2021/02/16 16:22:20 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "philo.h"

void				init_atomic_print(t_atomic_bool *print_guard)
{
	printf("%s", "");
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
		printf("%6llu ms | %3d | %s\n",
			get_current_time_ms() - g_worker_start, philo_idx + 1, msg);
	pthread_mutex_unlock(&g_print_guard.guard);
}

void				print_last_atomic_message(int philo_idx, const char *msg)
{
	pthread_mutex_lock(&g_print_guard.guard);
	if (g_print_guard.value)
		printf("%6llu ms | %3d | %s\n",
			get_current_time_ms() - g_worker_start, philo_idx + 1, msg);
	g_print_guard.value = false;
	pthread_mutex_unlock(&g_print_guard.guard);
}
