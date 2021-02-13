/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atomic.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/07 20:40:33 by handrow           #+#    #+#             */
/*   Updated: 2021/02/14 00:48:04 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool		get_atomic_bool(t_atomic_bool *atom)
{
	bool	val;

	sem_wait(atom->guard);
	val = atom->value;
	sem_post(atom->guard);
	return (val);
}

void		set_atomic_bool(t_atomic_bool *atom, bool value)
{
	sem_wait(atom->guard);
	atom->value = value;
	sem_post(atom->guard);
}

t_time_ms	get_atomic_time_ms(t_atomic_time_ms *atom)
{
	t_time_ms	val;

	sem_wait(atom->guard);
	val = atom->value;
	sem_post(atom->guard);
	return (val);
}

void		set_atomic_time_ms(t_atomic_time_ms *atom, t_time_ms value)
{
	sem_wait(atom->guard);
	atom->value = value;
	sem_post(atom->guard);
}
