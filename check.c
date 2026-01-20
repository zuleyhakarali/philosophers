/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarali <zkarali@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 11:34:47 by zkarali           #+#    #+#             */
/*   Updated: 2026/01/20 12:22:10 by zkarali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*for_philo(t_arg *arg)
{
	int	i;

	arg->philo = malloc(arg->num_of_philo * sizeof(t_philo));
	if (!arg->philo)
		return (NULL);
	i = 0;
	while (i < arg->num_of_philo)
	{
		arg->philo[i].r_fork = (i + 1) % arg->num_of_philo;
		arg->philo[i].l_fork = i;
		arg->philo[i].eat_c = 0;
		arg->philo[i].arg = arg;
		if (pthread_mutex_init(&arg->philo[i].last_eat_c, NULL) != 0)
			return (NULL);
		if (pthread_mutex_init(&arg->philo[i].m_c, NULL) != 0)
			return (NULL);
		i++;
	}
	return (arg->philo);
}

static int	for_check(t_arg *a, int i, long long time)
{
	long long	for_meal;
	long long	d_time;

	pthread_mutex_lock(&a->philo[i].last_eat_c);
	for_meal = a->philo[i].last_eat;
	pthread_mutex_unlock(&a->philo[i].last_eat_c);
	if (time >= for_meal && time - for_meal > a->die_time)
	{
		pthread_mutex_lock(&a->p_lock);
		pthread_mutex_lock(&a->dpn_lock);
		a->dead_philo_num = 1;
		d_time = time - a->start_time;
		if (d_time < 0)
			d_time = 0;
		printf("%lld %d died\n", d_time, i + 1);
		pthread_mutex_unlock(&a->dpn_lock);
		pthread_mutex_unlock(&a->p_lock);
		return (1);
	}
	return (0);
}

static int	eat_check(t_arg *a, int i)
{
	int	c;

	c = 0;
	while (i < a->num_of_philo)
	{
		pthread_mutex_lock(&a->philo[i].m_c);
		if (a->must_eat_c != -1 && a->philo[i].eat_c >= a->must_eat_c)
			c++;
		pthread_mutex_unlock(&a->philo[i].m_c);
		i++;
	}
	if (c == a->num_of_philo)
		return (1);
	return (0);
}

void	*check(void *arg)
{
	t_arg		*a;
	int			i;
	long long	time;

	a = (t_arg *)arg;
	usleep(1000);
	while (1)
	{
		i = 0;
		time = for_time();
		while (i < a->num_of_philo)
		{
			if (for_check(a, i, time) == 1)
				return (NULL);
			i++;
		}
		if (eat_check(a, 0) == 1)
			return (NULL);
		usleep(500);
	}
	return (NULL);
}
