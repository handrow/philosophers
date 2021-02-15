/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 22:28:44 by handrow           #+#    #+#             */
/*   Updated: 2021/02/14 01:34:09 by handrow          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>

#include "philo.h"

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
