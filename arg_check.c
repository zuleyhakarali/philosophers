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

long long	for_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL));
}

void	print(t_arg *a, int id, char *m)
{
	pthread_mutex_lock(&a->p_lock);
	pthread_mutex_lock(&a->dpn_lock);
	if (a->dead_philo_num == 0)
		printf("%lld %d %s\n", for_time() - a->start_time, id, m);
	pthread_mutex_unlock(&a->dpn_lock);
	pthread_mutex_unlock(&a->p_lock);
}
