/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atomic.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/07 20:40:33 by handrow           #+#    #+#             */
/*   Updated: 2021/02/13 22:48:41 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool		get_atomic_bool(t_atomic_bool *atom)
{
	bool	val;

	pthread_mutex_lock(&atom->guard);
	val = atom->value;
	pthread_mutex_unlock(&atom->guard);
	return (val);
}

void		set_atomic_bool(t_atomic_bool *atom, bool value)
{
	pthread_mutex_lock(&atom->guard);
	atom->value = value;
	pthread_mutex_unlock(&atom->guard);
}

t_time_ms	get_atomic_time_ms(t_atomic_time_ms *atom)
{
	t_time_ms	val;

	pthread_mutex_lock(&atom->guard);
	val = atom->value;
	pthread_mutex_unlock(&atom->guard);
	return (val);
}

void		set_atomic_time_ms(t_atomic_time_ms *atom, t_time_ms value)
{
	pthread_mutex_lock(&atom->guard);
	atom->value = value;
	pthread_mutex_unlock(&atom->guard);
}
