/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wahmed <wahmed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 12:33:10 by wahmed            #+#    #+#             */
/*   Updated: 2024/01/31 17:33:11 by wahmed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	dead(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_lock);
	if (*philo->dead == 1)
		return (pthread_mutex_unlock(philo->dead_lock), 1);
	pthread_mutex_unlock(philo->dead_lock);
	return (0);
}

static void	*philo_routine(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	if (philo->id % 2 == 1)
		ft_usleep(50);
	while (!dead(philo))
	{
		eating(philo);
		sleeping(philo);
		thinking(philo);
	}
	return (p);
}

int	thread_create(t_data *data, pthread_mutex_t *forks)
{
	
	pthread_t	observer;
	int			i;

	if (pthread_create(&observer, NULL, &monitor, data->philos) != 0)
		destroying("error thread creation", data, forks);
	i = 0;
	while (i < data->philos[0].philos_n)
	{
		if (pthread_create(&data->philos[i].thread, NULL, &philo_routine,
				&data->philos[i]) != 0)
			destroying("error thread creation", data, forks);
		i++;
	}
	i = 0;
	if (pthread_join(observer, NULL) != 0)
		destroying("error in joining threads", data, forks);
	while (i < data->philos[0].philos_n)
	{
		if (pthread_join(data->philos[i].thread, NULL) != 0)
			destroying("error in joining threads", data, forks);
		i++;
	}
	return (0);
}
