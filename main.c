/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarali <zkarali@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 11:34:54 by zkarali           #+#    #+#             */
/*   Updated: 2026/01/20 12:33:19 by zkarali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	for_mutex(t_arg *arg)
{
	if (pthread_mutex_init(&arg->p_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&arg->dpn_lock, NULL) != 0)
		return (1);
	if (for_fork(arg) == 1)
		return (1);
	return (0);
}

static int	for_thread(t_arg *arg, t_philo *philo)
{
	int	i;

	i = -1;
	arg->start_time = for_time();
	while (++i < arg->num_of_philo)
	{
		pthread_mutex_lock(&arg->philo[i].last_eat_c);
		arg->philo[i].last_eat = arg->start_time;
		pthread_mutex_unlock(&arg->philo[i].last_eat_c);
	}
	i = -1;
	while (++i < arg->num_of_philo)
	{
		if (pthread_create(&philo[i].t_id, NULL, ft, &philo[i]) != 0)
			return (1);
	}
	if (pthread_create(&arg->check, NULL, check, arg) != 0)
		return (1);
	pthread_join(arg->check, NULL);
	i = -1;
	while (++i < arg->num_of_philo)
		pthread_join(philo[i].t_id, NULL);
	return (0);
}

static void	for_destroy(t_arg *arg)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&arg->p_lock);
	while (i < arg->num_of_philo)
	{
		pthread_mutex_destroy(&arg->fork[i]);
		pthread_mutex_destroy(&arg->philo[i].last_eat_c);
		pthread_mutex_destroy(&arg->philo[i].m_c);
		i++;
	}
	pthread_mutex_destroy(&arg->dpn_lock);
	free(arg->fork);
	free(arg->philo);
	free(arg);
}

int	main(int ac, char **av)
{
	t_arg	*arg;
	t_philo	*philo;

	if (arg_check(ac, av) == 1)
	{
		write(1, "Error\n", 6);
		return (1);
	}
	arg = placement(ac, av);
	if (arg == NULL)
		return (1);
	if (for_mutex(arg) == 1)
		return (free(arg), 1);
	philo = for_philo(arg);
	if (philo == NULL)
	{
		for_free(arg, 1);
		return (1);
	}
	if (for_thread(arg, philo) == 1)
	{
		for_destroy(arg);
		return (1);
	}
	for_destroy(arg);
}
