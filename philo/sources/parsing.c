/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 11:55:16 by mcombeau          #+#    #+#             */
/*   Updated: 2022/07/02 16:52:57 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* contains_only_digits:
*	Checks if a string contains only digits 0 - 9.
*	Returns true (0) if the string only contains digits.
*	Returns false (1) if the string contains a character that is not a digit.
*/
static int	contains_only_digits(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/* integer_atoi:
*	Converts a digit-only string into a positive integer.
*	Returns the converted number between 0 and INT MAX.
*	Returns -1 if the converted number exceeds INT MAX.
*/
int	integer_atoi(char *str)
{
	unsigned long long int	nb;
	int						i;

	i = 0;
	nb = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	if (nb > INT_MAX || nb == 0)
		return (-1);
	return ((int)nb);
}

/* is_valid_input:
*	Checks if all required arguments are valid, i.e. is a string of
*	digits only, which does not exceed INT MAX.
*	Returns 1 if all arguments are valid, 0 if one of them is invalid.
*/
int	is_valid_input(int ac, char **av)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (!contains_only_digits(av[i]))
			return (0);
		if (integer_atoi(av[i]) == -1)
			return (0);
		i++;
	}
	return (1);
}
