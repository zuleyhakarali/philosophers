#include "philo.h"

void	for_free(t_arg *arg, int i)
{
	int	j;

	j = 0;
	pthread_mutex_destroy(&arg->p_lock);
	pthread_mutex_destroy(&arg->dpn_lock);
	if (i == 1)
	{
		while (j < arg->num_of_philo)
			pthread_mutex_destroy(&arg->fork[i++]);
		free(arg);
	}
}

int	for_fork(t_arg *arg)
{
	int	num;
	int	i;

	num = arg->num_of_philo;
	arg->fork = malloc(num * sizeof(pthread_mutex_t));
	if (!arg->fork)
	{
		for_free(arg, 0);
		return (1);
	}
	i = 0;
	while (i < num)
		pthread_mutex_init(&arg->fork[i++], NULL);
	return (0);
}

int	is_dead(t_arg *arg)
{
	int	dead;

	pthread_mutex_lock(&arg->dpn_lock);
	dead = arg->dead_philo_num;
	pthread_mutex_unlock(&arg->dpn_lock);
	return (dead);
}

void	for_destroy(t_arg *arg)
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
