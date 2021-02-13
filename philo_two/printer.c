/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 22:30:49 by handrow           #+#    #+#             */
/*   Updated: 2021/02/14 00:46:21 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "philo.h"

void				init_atomic_print(t_atomic_bool *print_guard)
{
	printf("%s", "");
	print_guard->value = true;
	print_guard->guard = sem_create(SEMNAME_PRINTER, 0, 1);
}

void				destroy_atomic_print(t_atomic_bool *print_guard)
{
	sem_close(print_guard->guard);
	sem_delete(SEMNAME_PRINTER, 0);
}

void				print_atomic_message(int philo_idx, const char *msg)
{
	sem_wait(g_print_guard.guard);
	if (g_print_guard.value)
		printf("%6llu ms | %3d | %s\n", get_current_time_ms() - g_worker_start, philo_idx + 1, msg);
	sem_post(g_print_guard.guard);
}

void				print_last_atomic_message(int philo_idx, const char *msg)
{
	sem_wait(g_print_guard.guard);
	if (g_print_guard.value)
		printf("%6llu ms | %3d | %s\n", get_current_time_ms() - g_worker_start, philo_idx + 1, msg);
	g_print_guard.value = false;
	sem_post(g_print_guard.guard);
}
