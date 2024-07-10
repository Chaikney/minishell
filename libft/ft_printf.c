/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 17:28:40 by chaikney          #+#    #+#             */
/*   Updated: 2023/07/28 17:28:44 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Wraps the puthex function for pointer output
static int	pf_do_ptr(void *nbr)
{
	int		put;

	put = 0;
	put += pf_putchar_fd('0', 1);
	put += pf_putchar_fd('x', 1);
	put += pf_puthex_fd((unsigned long) nbr, 'p', 1);
	return (put);
}

// Handle %s format codes.
// Take casted variable, catch (null) and send on to pf_putstr_fd
// Return number of chars printed
// NOTE (null) counts as the 6 chars you'd expect.
static int	pf_do_str(char *s)
{
	int	put;

	put = 0;
	if (!s)
		put = pf_putstr_fd("(null)", 1);
	else
		put = pf_putstr_fd(s, 1);
	return (put);
}

// Directive handling, i.e. chars after a %
// Receive the location in the format string and a variable to work with.
// Routes to the right transformation function and return a str to print.
// NOTE This cannot handle flags between % and letter.
// NOTE The pointer position here does *not* change it outside!
int	direct_output(const char *fchar, void *anything)
{
	int		put;

	put = 0;
	fchar++;
	if ((*(fchar) == 'i') || (*(fchar) == 'd'))
		put = pf_put_signed_fd((long) anything, 1);
	else if (*(fchar) == 's')
		put = pf_do_str((char *) anything);
	else if (*(fchar) == 'p')
		put = pf_do_ptr(anything);
	else if ((*(fchar) == 'x') || (*(fchar) == 'X'))
		put = pf_puthex_fd((long) anything, *(fchar), 1);
	else if (*(fchar) == 'c')
		put = pf_putchar_fd((long) anything, 1);
	else if (*(fchar) == 'u')
		put = pf_put_unsigned_fd((long) anything, 1);
	else
		put = -1;
	return (put);
}

// Receive input text and unknown number of parameters.
// Print text with interpolations, and return the number of chars printed.
int	ft_printf(const char *fstr, ...)
{
	va_list		codes;
	int			printed;

	va_start (codes, fstr);
	printed = 0;
	while (*fstr != '\0')
	{
		if (*fstr == '%')
		{
			if (*(fstr + 1) == '%')
				printed += pf_putchar_fd('%', 1);
			else
				printed += direct_output(fstr, va_arg (codes, void *));
			fstr++;
		}
		else
			printed += pf_putchar_fd(*fstr, 1);
		fstr++;
	}
	va_end (codes);
	return (printed);
}
