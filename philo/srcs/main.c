/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdebotte <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 13:17:45 by wdebotte          #+#    #+#             */
/*   Updated: 2022/05/07 03:46:18 by wdebotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_infos	infos;

	if (argc < 5 || argc > 6)
	{
		_putstr("Usage: ./philo \t[number of philosophers]\n\t\t[time to die]\
\n\t\t[time to eat]\n\t\t[time to sleep]\n\t\t[number of how many time \
each philosopher must eat(optionnal)]\nAll time must be in ms!\n");
		return (1);
	}
	if (set_infos(&infos, argc, argv) != 0)
		return (1);
	if (infos.must_eat == 0)
		return (_putstr("Philosophers aren't hungry!\n")
			&& exit_program(&infos, FALSE));
	if (set_philos(&infos) != 0)
		return (1);
	if (set_threads(&infos) != 0)
		return (1);
	return (exit_program(&infos, TRUE));
}
