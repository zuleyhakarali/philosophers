/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarali <zkarali@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 11:34:42 by zkarali           #+#    #+#             */
/*   Updated: 2026/01/20 12:50:50 by zkarali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *s)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	while (s[i] >= '0' && s[i] <= '9')
	{
		result = result * 10 + (s[i] - '0');
		i++;
	}
	return (result);
}

int	arg_check(int ac, char **av)
{
	int		i;
	int		j;
	long	val;

	i = 1;
	if (ac != 5 && ac != 6)
		return (1);
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (!(av[i][j] >= '0' && av[i][j] <= '9'))
				return (1);
			j++;
		}
		if (j > 10)
			return (1);
		val = ft_atoi(av[i]);
		if (val > 2147483647 || val <= 0)
			return (1);
		i++;
	}
	return (0);
}

t_arg	*placement(int ac, char **av)
{
	t_arg	*arg;

	arg = malloc(sizeof(t_arg));
	if (!arg)
		return (NULL);
	arg->num_of_philo = ft_atoi(av[1]);
	arg->die_time = ft_atoi(av[2]);
	arg->eat_time = ft_atoi(av[3]);
	arg->sleep_time = ft_atoi(av[4]);
	if (ac == 5)
		arg->must_eat_c = -1;
	else
		arg->must_eat_c = ft_atoi(av[5]);
	arg->dead_philo_num = 0;
	return (arg);
}
