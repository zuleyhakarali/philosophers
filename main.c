#include "philo.h"

static void eating(t_philo *p)
{
    if ((p->l_fork + 1) % 2 == 0)
    {
        pthread_mutex_lock(&p->arg->fork[p->r_fork]);
        pthread_mutex_lock(&p->arg->fork[p->l_fork]);
    }
    else
    {
        pthread_mutex_lock(&p->arg->fork[p->l_fork]);
        pthread_mutex_lock(&p->arg->fork[p->r_fork]);
    }
    print(p->arg, p->l_fork + 1, "has taken a fork");
    print(p->arg, p->l_fork + 1, "has taken a fork");
    pthread_mutex_lock(&p->meal);
    p->last_eat = for_time();
    pthread_mutex_unlock(&p->meal);
    print(p->arg, p->l_fork + 1, "is eating");
    usleep(p->arg->eat_time * 1000);
    pthread_mutex_lock(&p->m_c);
    p->eat_c++;
    pthread_mutex_unlock(&p->m_c);
    pthread_mutex_unlock(&p->arg->fork[p->r_fork]);
    pthread_mutex_unlock(&p->arg->fork[p->l_fork]);
}

static void thinking(t_philo *p)
{
    long t_time;
    long max_t;

    print(p->arg, p->l_fork + 1, "is thinking");
    if (p->arg->num_of_philo % 2 != 0)
    {
        if (p->arg->eat_time > p->arg->sleep_time)
            max_t = p->arg->eat_time;
        else
            max_t = p->arg->sleep_time;
        t_time = (max_t * 2) / 5;
        if (t_time > 100)
            t_time = 100;
        if (t_time < 1)
            t_time = 1;
        usleep(t_time * 1000);
    }
}

static int for_extra(t_philo *p)
{
    if (one_philo(p) == 1)
        return (1);
    if ((p->l_fork + 1) % 2 == 0)
        usleep(p->arg->eat_time * 500);
    return (0);
}

void *ft(void *philo)
{
    t_philo *p;

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
            break;
        }
        pthread_mutex_unlock(&p->m_c);
        if (is_dead(p->arg) == 1 || is_over(p->arg) == 1)
            break;
        print(p->arg, p->l_fork + 1, "is sleeping");
        usleep(p->arg->sleep_time * 1000);
        if (is_dead(p->arg) == 1)
            break;
        thinking(p);
    }
    return (NULL);
}

int main(int ac, char **av)
{
    t_arg *arg;
    t_philo *philo;
    int i;

    arg_check(ac, av);
    arg = placement(ac, av);
    pthread_mutex_init(&arg->print_w, NULL);
    pthread_mutex_init(&arg->dpn_lock, NULL);
    pthread_mutex_init(&arg->c_lock, NULL);
    for_fork(arg);
    philo = for_philo(arg);
    for_thread(arg, philo);
    pthread_join(arg->check, NULL);
    i = -1;
    while (++i < arg->num_of_philo)
        pthread_join(philo[i].t_id, NULL);
    for_destroy(arg);
    free(arg->fork);
    free(arg->philo);
    free(arg);
}
