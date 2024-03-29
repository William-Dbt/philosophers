/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdebotte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 14:39:54 by wdebotte          #+#    #+#             */
/*   Updated: 2022/05/07 06:04:39 by wdebotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	set_infos(t_infos *infos, int argc, char **argv)
{
	infos->n_philos = _atoli(argv[1]);
	if (infos->n_philos < 1)
		return (_putstr("Args error: Philosophers must be > than 0, only \
composed by digits and must not overflow.\n"));
	infos->time_die = _atoli(argv[2]);
	infos->time_eat = _atoli(argv[3]);
	infos->time_sleep = _atoli(argv[4]);
	if (infos->time_die <= 0 || infos->time_eat <= 0 || infos->time_sleep <= 0)
		return (_putstr("Args error: All times must be > than 0, only composed \
by digits and must not overflow.\n"));
	infos->must_eat = -1;
	infos->all_eaten = FALSE;
	if (argc == 6)
	{
		infos->must_eat = _atoli(argv[5]);
		if (infos->must_eat < 0)
			return (_putstr("Args error: must_eat must be > or = than 0, only  \
composed by digits and must not overflow.\n"));
	}
	infos->thr_alive = TRUE;
	if (pthread_mutex_init(&infos->mutex, NULL) != 0
		|| pthread_mutex_init(&infos->mutex_init, NULL) != 0
		|| pthread_mutex_init(&infos->mutex_message, NULL) != 0)
		return (_putstr("Mutex error: Can't init mutex.\n"));
	return (0);
}

int	set_philos_mutexes(t_infos *infos)
{
	int	i;

	i = -1;
	while (++i < infos->n_philos)
		if (pthread_mutex_init(&infos->philos[i].mutex, NULL) != 0
			|| pthread_mutex_init(&infos->philos[i].mutex_fork, NULL) != 0)
			return (_putstr("Mutex error: Can't init mutex.\n"));
	if (infos->n_philos == 1)
	{
		infos->philos[0].rphilo = NULL;
		return (0);
	}
	i = -1;
	while (++i < infos->n_philos)
	{
		if (i == infos->n_philos - 1)
			infos->philos[i].rphilo = &infos->philos[0];
		else
			infos->philos[i].rphilo = &infos->philos[i + 1];
	}
	return (0);
}

int	set_philos(t_infos *infos)
{
	int	i;

	infos->philos = malloc(sizeof(t_philo) * infos->n_philos);
	if (infos->philos == NULL)
		return (_putstr("Malloc error: Can't malloc philo's tab.\n"));
	i = -1;
	while (++i < infos->n_philos)
	{
		infos->philos[i].id = i + 1;
		if (infos->must_eat != -1)
			infos->philos[i].n_eat = 0;
		else
			infos->philos[i].n_eat = -1;
		infos->philos[i].infos = infos;
	}
	return (set_philos_mutexes(infos));
}

int	set_threads(t_infos *infos)
{
	int	i;

	pthread_mutex_lock(&infos->mutex_init);
	i = -1;
	while (++i < infos->n_philos)
	{
		infos->philos[i].start_eat = get_time();
		if (pthread_create(&infos->philos[i].thread, NULL, &routine,
				&infos->philos[i]) != 0)
			return (_putstr("Thread error: Can't create thread.\n"));
	}
	infos->time_start = get_time();
	pthread_mutex_unlock(&infos->mutex_init);
	check_died_philos(infos);
	i = -1;
	while (++i < infos->n_philos)
		if (pthread_join(infos->philos[i].thread, NULL) != 0)
			return (_putstr("Thread error: Can't join thread.\n"));
	return (0);
}
