#include "philo.h"

void for_thread(t_arg *arg, t_philo *philo)
{
    int i;

    i = 0;
    arg->start_time = for_time();
    while (i < arg->num_of_philo)
    {
        pthread_mutex_lock(&arg->philo[i].meal);
        arg->philo[i].last_eat = arg->start_time;
        pthread_mutex_unlock(&arg->philo[i].meal);
        pthread_create(&philo[i].t_id, NULL, ft, &philo[i]);
        i++;
    }
    pthread_create(&arg->check, NULL, check, arg);
}

void for_destroy(t_arg *arg)
{
    int i;

    i = 0;
    pthread_mutex_destroy(&arg->print_w);
    while (i < arg->num_of_philo)
    {
        pthread_mutex_destroy(&arg->fork[i]);
        pthread_mutex_destroy(&arg->philo[i].meal);
        pthread_mutex_destroy(&arg->philo[i].m_c);
        i++;
    }
    pthread_mutex_destroy(&arg->c_lock);
}
