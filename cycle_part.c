#include "philo.h"

static void	eating(t_philo *p)
{
	pthread_mutex_lock(&p->arg->fork[p->l_fork]);
	pthread_mutex_lock(&p->arg->fork[p->r_fork]);
	print(p->arg, p->l_fork + 1, "has taken a fork");
	print(p->arg, p->l_fork + 1, "has taken a fork");
	pthread_mutex_lock(&p->last_eat_c);
	p->last_eat = for_time();
	pthread_mutex_unlock(&p->last_eat_c);
	print(p->arg, p->l_fork + 1, "is eating");
	usleep(p->arg->eat_time * 1000);
	pthread_mutex_lock(&p->m_c);
	p->eat_c++;
	pthread_mutex_unlock(&p->m_c);
	pthread_mutex_unlock(&p->arg->fork[p->r_fork]);
	pthread_mutex_unlock(&p->arg->fork[p->l_fork]);
}

static void	thinking(t_philo *p)
{
	long	t_time;

	if (p->arg->num_of_philo % 2 != 0)
	{
		t_time = (p->arg->eat_time + p->arg->sleep_time) / 2;
		if (t_time > 100)
			t_time = 100;
		print(p->arg, p->l_fork + 1, "is thinking");
		usleep(t_time * 1000);
	}
}

static int	one_philo(t_philo *p)
{
	if (p->arg->num_of_philo == 1)
	{
		pthread_mutex_lock(&p->arg->fork[p->r_fork]);
		print(p->arg, p->l_fork + 1, "has taken a fork");
		while (1)
		{
			pthread_mutex_lock(&p->arg->dpn_lock);
			if (p->arg->dead_philo_num == 1)
			{
				pthread_mutex_unlock(&p->arg->dpn_lock);
				pthread_mutex_unlock(&p->arg->fork[p->r_fork]);
				return (1);
			}
			pthread_mutex_unlock(&p->arg->dpn_lock);
			usleep(100);
		}
	}
	return (0);
}

static int	for_extra(t_philo *p)
{
	if (one_philo(p) == 1)
		return (1);
	if ((p->l_fork + 1) % 2 == 0)
		usleep(p->arg->eat_time * 500);
	return (0);
}

void	*ft(void *philo)
{
	t_philo	*p;

	p = (t_philo *)philo;
	if (for_extra(p) == 1)
		return (NULL);
	while (1)
	{
		eating(p);
		pthread_mutex_lock(&p->m_c);
		if (p->arg->must_eat_c != -1 && p->eat_c >= p->arg->must_eat_c)
		{
			pthread_mutex_unlock(&p->m_c);
			break ;
		}
		pthread_mutex_unlock(&p->m_c);
		if (is_dead(p->arg) == 1)
			break ;
		print(p->arg, p->l_fork + 1, "is sleeping");
		usleep(p->arg->sleep_time * 1000);
		if (is_dead(p->arg) == 1)
			break ;
		thinking(p);
	}
	return (NULL);
}
