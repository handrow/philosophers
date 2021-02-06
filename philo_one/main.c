/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: handrow <handrow@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 18:26:27 by handrow           #+#    #+#             */
/*   Updated: 2021/02/06 19:48:27 by handrow          ###   ########.fr       */
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

typedef int64_t		t_time_ms;

enum e_setting_idx
{
	PHILO_NUM_ARG_IDX = 1,
	PHILO_TTD_ARG_IDX = 2,
	PHILO_TTE_ARG_IDX = 3,
	PHILO_TTS_ARG_IDX = 4,
	PHILO_MEAL_COUNT_ARG_IDX = 5,

	PHILO_ARG_LIMIT_MIN = 5,
	PHILO_ARG_LIMIT_MAX = 6
};

struct s_settings
{
	int				philo_num;
	int				meal_count;
	t_time_ms	 	time_to_eat;
	t_time_ms	 	time_to_sleep;
	t_time_ms	 	time_to_die;
};

struct s_settings	g_settings;

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

int					main(int ac, char *av[])
{
	if (fill_settings(&g_settings, ac, av) == false)
	{
		printf("Invalid arguments\n");
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
