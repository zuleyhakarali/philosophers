#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdio.h>

typedef struct s_arg t_arg;

typedef struct s_philo
{
    pthread_t t_id;
    int l_fork;
    int r_fork;
    int eat_c;
    long last_eat;
    pthread_mutex_t meal;
    t_arg *arg;
} t_philo;

typedef struct s_arg
{
    int num_of_philo;
    long eat_time;
    long die_time;
    long sleep_time;
    int must_eat_c;
    long start_time;
    int dead_philo_num;
    int ate;
    pthread_mutex_t print_w;
    pthread_mutex_t *fork;
    pthread_mutex_t dpn_lock;
    pthread_mutex_t c_lock;
    pthread_t check;
    t_philo *philo;
} t_arg;

int main(int ac, char **av);
void arg_check(int ac, char **av);
void error(void);
int	ft_atoi(const char *nptr);
long for_time(void);
t_arg *placement(int ac, char **av);
void for_fork(t_arg *arg);
t_philo *for_philo(t_arg *arg);
void *check(void *arg);
void for_sleep(long long time, t_arg *a);

#endif