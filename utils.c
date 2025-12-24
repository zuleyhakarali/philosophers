#include "philo.h"

t_arg *placement(int ac, char **av)
{
    t_arg *arg;

    arg = malloc(sizeof(t_arg));
    if (!arg)
        error();
    arg->num_of_philo = ft_atoi(av[1]);
    arg->die_time = ft_atoi(av[2]);
    arg->eat_time = ft_atoi(av[3]);
    arg->sleep_time = ft_atoi(av[4]);
    if (ac == 5)
        arg->must_eat_c = 0;
    else
        arg->must_eat_c = ft_atoi(av[5]);
    if (arg->die_time <= 60 || arg->eat_time <= 60 ||
        arg->sleep_time <= 60)
        error();
    arg->dead_philo_num = 0;
    arg->start_time = for_time();
    arg->ate = 0;
    return (arg);
}

void for_fork(t_arg *arg)
{
    int num;
    int i;

    num = arg->num_of_philo;
    arg->fork = malloc(num * sizeof(pthread_mutex_t));
    if (!arg->fork)
        error();
    i = 0;
    while (i < num)
        pthread_mutex_init(&arg->fork[i++], NULL);
}

t_philo *for_philo(t_arg *arg)
{
    int i;

    arg->philo = malloc(arg->num_of_philo * sizeof(t_philo));
    if (!arg->philo)
        error();
    i = 0;
    while (i < arg->num_of_philo)
    {
        arg->philo[i].r_fork = (i + 1) % arg->num_of_philo;
        arg->philo[i].l_fork = i;
        arg->philo[i].eat_c = 0;
        arg->philo[i].last_eat = arg->start_time;
        arg->philo[i].arg = arg;
        pthread_mutex_init(&arg->philo[i].meal, NULL);
        i++;
    }
    return (arg->philo);
}

static int for_check(t_arg *a, int i, long time)
{
    long for_meal;

    pthread_mutex_lock(&a->philo[i].meal);
    for_meal = a->philo[i].last_eat;
    pthread_mutex_unlock(&a->philo[i].meal);
    if (time - for_meal > a->die_time)
    {
        pthread_mutex_lock(&a->print_w);
        pthread_mutex_lock(&a->dpn_lock);
        a->dead_philo_num = 1;
        printf("%ld %d died\n", time - a->start_time, i + 1);
        pthread_mutex_unlock(&a->dpn_lock);
        pthread_mutex_unlock(&a->print_w);
        return (1);
    }
    return (0);
}

void *check(void *arg)
{
    t_arg *a;
    int i;
    int c;
    long time;

    a = (t_arg *)arg;
    while (1)
    {
        time = for_time();
        i = 0;
        c = 0;
        while (i < a->num_of_philo)
        {
            if (for_check(a, i, time) == 1)
                return (NULL);
            if (a->must_eat_c != 0 && a->philo[i].eat_c >= a->must_eat_c)
                c++;
            i++;
        }
        if (c == a->num_of_philo)
        {
            pthread_mutex_lock(&a->c_lock);
            a->ate = 1;
            pthread_mutex_unlock(&a->c_lock);
            return (NULL);
        }
        usleep(200);
    }
    return (NULL);
}
