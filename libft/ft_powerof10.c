/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_powerof10.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 18:12:36 by chaikney          #+#    #+#             */
/*   Updated: 2023/08/23 18:12:42 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Returns 10 to the power of n
// NOTE n has to be >0 I could improve that except that needs a float return.
int	ft_powerof10(int n)
{
	int	i;
	int	answer;

	answer = 10;
	if (n == 0)
		return (1);
	i = 1;
	while (i++ < n)
		answer = answer * 10;
	return (answer);
}
