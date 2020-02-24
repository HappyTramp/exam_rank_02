/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exam <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 17:21:15 by exam              #+#    #+#             */
/*   Updated: 2020/02/24 18:33:59 by exam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

int ft_strlen(char *s)
{
	int i;
	
	i = 0;
	while (s[i])
		i++;
	return (i);
}

char *ft_strcpy(char *dest, char *src)
{
	char	*d;
	
	d = dest;
	while (*src)
		*dest++ = *src++;
	*dest = '\0';
	return (d);
}

char *ft_strncpy(char *dest, char *src, int n)
{
	char	*d;

	d = dest;
	while (*src && n-- > 0)
		*dest++ = *src++;
	*dest = '\0';
	return (d);
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char *s)
{
	write(1, s, ft_strlen(s));
}

void	ft_putnbr_base(long long n, char *base, int radix)
{
	if (n < 0)
	{
		ft_putchar('-');
		ft_putnbr_base(-n, base, radix);
		return ;
	}
	if (n > radix - 1)
		ft_putnbr_base(n / radix, base, radix);
	ft_putchar(base[n % radix]);
}

int		nbrlen(long long n, int radix)
{
	int i;

	if (n == 0)
		return (1);
	i = 0;
	if (n < 0)
	{
		i++;
		n = -n;
	}
	while (n > 0)
	{
		n /= radix;
		i++;
	}
	return (i);
}

char	*ft_strndup(char *s, int n)
{
	char	*d;

	if ((d = malloc(sizeof(char) * (n + 1))) == NULL)
		return (NULL);
	return (ft_strncpy(d, s, n));
}

int		ft_atoi_skip(char **s)
{
	long	n;

	n = 0;
	while (**s >= '0' && **s <= '9')
	{
		n *= 10;
		n += **s - '0';
		(*s)++;
	}
	return ((int)n);
}

typedef struct
{
	int		precision;
	int		width;
	char	specifier;
}		t_parsing;

char	*parse(char *fmt, t_parsing *parsing)
{
	parsing->width = -1;
	parsing->precision = -1;
	if (*fmt >= '0' && *fmt <= '9')
		parsing->width = ft_atoi_skip(&fmt);
	if (*fmt == '.')
		fmt++;
	if (*fmt >= '0' && *fmt <= '9')
		parsing->precision = ft_atoi_skip(&fmt);
	parsing->specifier = *fmt++;
	return (fmt);
}

int	convert_s(va_list ap, t_parsing *parsing)
{
	char	*tmp;
	int		len;
	char	s[400000];
	int		counter;

	tmp = va_arg(ap, char*);
	if (tmp == NULL)
		ft_strcpy(s, "(null)");
	else
		ft_strcpy(s, tmp);
	len = ft_strlen(s);
	if (parsing->width != -1 && parsing->precision != -1)
	{
		while (len > parsing->precision)
			len--;
		s[len] = 0;
		counter = parsing->width > len ? parsing->width : len;
		while (parsing->width-- > len)
			ft_putchar(' ');
		ft_putstr(s);
		return (counter);
	}
	else if (parsing->width != -1)
	{
		counter = parsing->width > len ? parsing->width : len;
		while (parsing->width-- > len)
			ft_putchar(' ');
		ft_putstr(s);
		return (counter);
	}
	else if (parsing->precision != -1)
	{
		counter = parsing->precision < len ? parsing->precision : len;
		while (len-- > parsing->precision)
			;
		s[len + 1] = 0;
		ft_putstr(s);
		return (counter);
	}
	else
		ft_putstr(s);
	return (len);
}

int	convert_d(va_list ap, t_parsing *parsing)
{
	long	n;
	int len;
	int counter;

	n = (long)va_arg(ap, int);
	len = nbrlen(n, 10);
	if (parsing->width != -1 && parsing->precision != -1)
	{
		counter = parsing->width > len ? parsing->width : len;
		while (parsing->width > (parsing->precision > len ? (n < 0 ? parsing->precision + 1 : parsing->precision) : len))
		{
			ft_putchar(' ');
			parsing->width--;
		}
		if (n < 0)
		{
			ft_putchar('-');
			n = -n;
			len--;
		}
		counter = counter > parsing->precision ? counter :parsing->precision;
		while (parsing->precision-- > len)
			ft_putchar('0');
		ft_putnbr_base(n, "0123456789", 10);
		return (counter);
	}
	else if (parsing->width != -1)
	{
		counter = parsing->width > len ? parsing->width : len;
		while (parsing->width-- > len)
			ft_putchar(' ');
		ft_putnbr_base(n, "0123456789", 10);
		return (counter);
	}
	else if (parsing->precision != -1)
	{
		counter = 0;
		if (n < 0)
		{
			ft_putchar('-');
			n = -n;
			len--;
			counter = 1;
		}
		counter += parsing->precision > len ? parsing->precision : len;
		while (parsing->precision-- > len)
			ft_putchar('0');
		ft_putnbr_base(n, "0123456789", 10);
		return (counter);
	}
	ft_putnbr_base(n, "0123456789", 10);
	return (len);
}

int	convert_x(va_list ap, t_parsing *parsing)
{
	unsigned int	n;
	int				len;
	int				counter;

	n = va_arg(ap, unsigned int);
	len = nbrlen(n, 16);
	if (parsing->width != -1 && parsing->precision != -1)
	{
		counter = parsing->width > len ? parsing->width : len;
		while (parsing->width-- > (parsing->precision > len ? parsing->precision : len))
			ft_putchar(' ');
		counter = counter > parsing->precision ? counter :parsing->precision;
		counter = counter > parsing->precision ? counter :parsing->precision;
		counter = counter > parsing->precision ? counter :parsing->precision;
		counter = counter > parsing->precision ? counter :parsing->precision;
		while (parsing->precision-- > len)
			ft_putchar('0');
		ft_putnbr_base(n, "0123456789abcdef", 16);
		return (counter);
	}
	else if (parsing->width != -1)
	{
		counter = parsing->width > len ? parsing->width : len;
		while (parsing->width-- > len)
			ft_putchar(' ');
		ft_putnbr_base(n, "0123456789abcdef", 16);
		return (counter);
	}
	else if (parsing->precision != -1)
	{
		counter = len > parsing->precision ? len :parsing->precision;
		while (parsing->precision-- > len)
			ft_putchar('0');
		ft_putnbr_base(n, "0123456789abcdef", 16);
		return (counter);
	}
	ft_putnbr_base(n, "0123456789abcdef", 16);
	return (len);
}

int ft_printf(const char *fmt, ... )
{
	t_parsing	parsing;
	int 		counter;
	va_list		ap;

	va_start(ap, fmt);
	counter = 0;
	while (*fmt)
	{
		if (*fmt == '%')
		{
			fmt++;
			fmt = parse((char*)fmt, &parsing);
//			printf("%d %d\n", parsing.width, parsing.precision);
			if (parsing.specifier == 's')
				counter += convert_s(ap, &parsing);
			else if (parsing.specifier == 'd')
				counter += convert_d(ap, &parsing);
			else if (parsing.specifier == 'x')
				counter += convert_x(ap, &parsing);
			else
				return (-1);
		}
		else
		{
			ft_putchar(*fmt++);
			counter++;
		}
	}
	return (counter);
}
