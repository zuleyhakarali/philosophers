#include "philo.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	result;
	int	sign;

	i = 0;
	result = 0;
	sign = 1;
	while ((nptr[i] == ' ') || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if ((nptr[i] == '-') || (nptr[i] == '+'))
	{
		if (nptr[i] == '-')
			sign *= -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
		result = result * 10 + (nptr[i++] - '0');
	return (sign * result);
}

void error(void)
{
    write(2, "Error\n", 6);
    exit(1);
}

static void args_check(char *s)
{
    int i;
    int val;

    i = 0;
    while (s[i])
    {
        if (!(s[i] >= '0' && s[i] <= '9'))
            error();
        i++;
    }
    if (i > 10)
        error();
    val = ft_atoi(s);
    if (val > 2147483647 || val < 0)
        error();
}

void arg_check(int ac, char **av)
{
    int i;
    int val;

    i = 1;
    if (ac != 5 && ac != 6)
        error();
    while (av[i])
        args_check(av[i++]);
    val = ft_atoi(av[1]);
    if (val <= 0 || val > 200)
        error();
}

long for_time(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
