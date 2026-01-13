#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdio.h>

typedef struct s_philo
{
	pthread_t		t_id;
	int				l_fork;
	int				r_fork;
	int				eat_c;
	long long		last_eat;
	pthread_mutex_t	last_eat_c;
	pthread_mutex_t	m_c;
	struct s_arg	*arg;
}	t_philo;

typedef struct s_arg
{
	int				num_of_philo;
	long			eat_time;
	long			die_time;
	long			sleep_time;
	int				must_eat_c;
	long long		start_time;
	int				dead_philo_num;
	pthread_mutex_t	p_lock;
	pthread_mutex_t	*fork;
	pthread_mutex_t	dpn_lock;
	pthread_t		check;
	t_philo			*philo;
}	t_arg;

int			main(int ac, char **av);
int			arg_check(int ac, char **av);
void		error(void);
int			ft_atoi(const char *s);
long long	for_time(void);
t_arg		*placement(int ac, char **av);
int			for_fork(t_arg *arg);
t_philo		*for_philo(t_arg *arg);
void		*check(void *arg);
void		*ft(void *philo);
void		print(t_arg *a, int id, char *m);
int			is_dead(t_arg *arg);
void		for_destroy(t_arg *arg);
void		for_free(t_arg *arg, int i);

#endif