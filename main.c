#include "philo.h"

static int print(t_arg *a, int id, char *m)
{
    int p;

    (void)m;
    (void)id;
    p = 0;
    pthread_mutex_lock(&a->print_w);
    pthread_mutex_lock(&a->dpn_lock);
    if (!a->dead_philo_num)
    {
        printf("%ld %d %s\n", for_time() - a->start_time, id, m);
        p = 1;
    }
    pthread_mutex_unlock(&a->print_w);
    pthread_mutex_unlock(&a->dpn_lock);
    return (p);
}

static int one_philo(t_philo *p)
{
    if (p->arg->num_of_philo == 1)
    {
        p->last_eat = for_time();
        pthread_mutex_lock(&p->arg->fork[p->r_fork]);
        print(p->arg, p->l_fork + 1, "has taken a fork");
        while (!p->arg->dead_philo_num)
            usleep(100);
        pthread_mutex_unlock(&p->arg->fork[p->r_fork]);
        return (1);
    }
    return (0);
}

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
    pthread_mutex_lock(&p->meal);
    p->last_eat = for_time();
    pthread_mutex_unlock(&p->meal);
    print(p->arg, p->l_fork + 1, "has taken a fork");
    print(p->arg, p->l_fork + 1, "has taken a fork");
    print(p->arg, p->l_fork + 1, "is eating");
    usleep(p->arg->eat_time * 1000);
    p->eat_c++;
    pthread_mutex_unlock(&p->arg->fork[p->r_fork]);
    pthread_mutex_unlock(&p->arg->fork[p->l_fork]);
}

static int is_dead(t_arg *arg)
{
    int dead;

    pthread_mutex_lock(&arg->dpn_lock);
    dead = arg->dead_philo_num;
    pthread_mutex_unlock(&arg->dpn_lock);
    if (dead == 1)
        return (1);
    return (0);
}

static int is_over(t_arg *arg)
{
    int over;

    pthread_mutex_lock(&arg->c_lock);
    over = arg->ate;
    pthread_mutex_unlock(&arg->c_lock);
    if (over == 1)
        return (1);
    return (0);
}

static void *ft(void *philo)
{
    t_philo *p;

    p = (t_philo *)philo;
    pthread_mutex_lock(&p->meal);
    p->last_eat = for_time();
    pthread_mutex_unlock(&p->meal);
    if (one_philo(p) == 1)
        return (NULL);
    if (p->arg->num_of_philo % 2 == 0)
        usleep(500);
    while (1)
    {
        eating(p);
        if (p->arg->must_eat_c != -1 && p->arg->must_eat_c == p->eat_c)
            break;
        if (is_dead(p->arg) == 1 || is_over(p->arg) == 1)
            break;
        print(p->arg, p->l_fork + 1, "is sleeping");
        usleep(p->arg->sleep_time + 1000);
        if (is_dead(p->arg) == 1 || is_over(p->arg) == 1)
            break;
        print(p->arg, p->l_fork + 1, "is thinking");
        if (p->arg->num_of_philo % 2 == 0)
            usleep(100);
    }
    return (NULL);
}

static void for_thread(t_arg *arg, t_philo *philo)
{
    int i;

    i = 0;
    while (i < arg->num_of_philo)
    {
        pthread_create(&philo[i].t_id, NULL, ft, &philo[i]);
        usleep(100);
        i++;
    }
    pthread_create(&arg->check, NULL, check, arg);
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
    arg->start_time = for_time();
    for_thread(arg, philo);
    pthread_join(arg->check, NULL);
    i = -1;
    while (++i < arg->num_of_philo)
        pthread_join(philo[i].t_id, NULL);
    pthread_mutex_destroy(&arg->print_w);
    i = -1;
    while (++i < arg->num_of_philo)
        pthread_mutex_destroy(&arg->fork[i]);
    pthread_mutex_destroy(&arg->c_lock);
    free(arg->fork);
    free(arg->philo);
    free(arg);
}
