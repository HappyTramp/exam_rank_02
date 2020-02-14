/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exam <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 16:57:09 by exam              #+#    #+#             */
/*   Updated: 2020/02/14 18:54:02 by exam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

#include <stdio.h>
void	ft_putchar(char c)
{
	write(1, &c, 1);
}

int		ft_strlen(char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	ft_putstr(char *s)
{
	write(1, s, ft_strlen(s));
}

char	*ft_strcpy(char *dest, char *src)
{
	char *s;

	s = dest;
	while (*src)
		*dest++ = *src++;
	*dest = '\0';
	return (s);
}

char	*ft_strdup(char *s)
{
	char	*clone;

	if ((clone = malloc(ft_strlen(s) + 1)) == NULL)
		return (NULL);
	ft_strcpy(clone, s);
	return (clone);
}

char	*ft_strcat(char *dest, char *src)
{
	char *s;

	s = dest;
	while (*dest)
		dest++;
	while (*src)
		*dest++ = *src++;
	*dest = '\0';
	return (s);
}

static int	st_itoa_base_count(long num, int radix)
{
	int	counter;

	counter = 0;
	if (num <= 0)
		counter = 1;
	while (num > 0)
	{
		num /= radix;
		counter++;
	}
	return (counter);
}

char	*ft_itoa_base(long long num, char *base)
{
	int		radix;
	char	*s;
	int		i;
	int		is_neg;

	radix = ft_strlen(base);
	if ((s = malloc(st_itoa_base_count(num, radix) + 1)) == NULL)
		return (NULL);
	s[st_itoa_base_count(num, radix)] = '\0';
	is_neg = 0;
	if (num < 0)
	{
		s[0] = '-';
		num = -num;
		is_neg = 1;
	}
	i = st_itoa_base_count(num, radix);
	if (num == 0)
	{
		s[0] = base[0];
		return (s);
	}
	while (i >= is_neg ? 1 : 0)
	{
		i--;
		s[is_neg ? i + 1 : i] = base[num % radix];
		num /= radix;
	}
	return (s);
}

int		ft_atoi_skip(char **s)
{
	long	num;

	num = 0;
	while (**s >= '0' && **s <= '9')
	{
		num *= 10;
		num += **s - '0';
		(*s)++;
	}
	return ((int)num);
}

typedef struct
{
	int	precision;
	int width;
}	t_parsing;

char	*parse(t_parsing *parsing, char *fmt)
{
	parsing->precision = -1;
	parsing->width = -1;

	if (*fmt >= '0' && *fmt <= '9')
		parsing->width = ft_atoi_skip(&fmt);
	if (*fmt == '.')
	{
		fmt++;
		parsing->precision = ft_atoi_skip(&fmt);
	}
	return (fmt);
}

int	convert_str(va_list ap, t_parsing *parsing)
{
	int		written;
	char	*tmp;
	char	*str;
	int		is_null;

	str = va_arg(ap, char*);
	is_null = str == NULL;
	if (str == NULL)
		if ((str = ft_strdup("(null)")) == NULL)
			return (-1);
	if (parsing->precision != -1 && ft_strlen(str) > parsing->precision)
	{
		if ((tmp = ft_strdup(str)) == NULL)
			return (-1);
		tmp[parsing->precision] = '\0';
		str = tmp;
	}
	written = ft_strlen(str);
	if (parsing->width != -1)
	{
		while (ft_strlen(str) < parsing->width)
		{
			ft_putchar(' ');
			parsing->width--;
			written++;
		}
	}
	ft_putstr(str);
	if (parsing->precision != -1 || is_null)
		free(str);
	return (written);
}

int	convert_d(va_list ap, t_parsing *parsing)
{
	int		i;
	int		len;
	long int		num;
	char	*s;
	char	*tmp;

	num = va_arg(ap, int);
	if ((s = ft_itoa_base(num, "0123456789")) == NULL)
		return (-1);
	len = ft_strlen(s);
	if (parsing->precision != -1 && len < parsing->precision)
	{
		if ((tmp = malloc(parsing->precision + 10)) == NULL)
			return (-1);
		ft_strcpy(tmp + parsing->precision - len, s);
		i = 0;
		while (i <  parsing->precision - len)
		{
			tmp[i] = '0';
			i++;
		}
		s = tmp;
	}
	len = ft_strlen(s);
	if (parsing->width != -1 && len < parsing->width)
	{
		while (len < parsing->width)
		{
			ft_putchar(' ');
			len++;
		}
	}
	ft_putstr(s);
	free(s);
	return (len);
}

int	convert_x(va_list ap, t_parsing *parsing)
{
	int				i;
	int				len;
	long unsigned int	num;
	char			*s;
	char			*tmp;

	num = va_arg(ap, unsigned int);
	if ((s = ft_itoa_base(num, "0123456789abcdef")) == NULL)
		return (-1);
	len = ft_strlen(s);
	if (parsing->precision != -1 && len < parsing->precision)
	{
		if ((tmp = malloc(parsing->precision + 1)) == NULL)
			return (-1);
		ft_strcpy(tmp + parsing->precision - len, s);
		i = 0;
		while (i <  parsing->precision - len)
		{
			tmp[i] = '0';
			i++;
		}
		free(s);
		s = tmp;
	}
	len = ft_strlen(s);
	if (parsing->width != -1 && len < parsing->width)
	{
		while (len < parsing->width)
		{
			ft_putchar(' ');
			len++;
		}
	}
	ft_putstr(s);
	if (parsing->precision == -1)
		free(s);
//	free(s);
	return (len);
}

int ft_printf(const char *fmt, ... )
{
	t_parsing	parsing;
	va_list		ap;
	int			i;
	int			tmp;

	va_start(ap, fmt);
	i = 0;
	while (*fmt)
	{
		if (*fmt == '%')
		{
			fmt = parse(&parsing, (char*)++fmt);
			if (*fmt == 's')
				i += convert_str(ap, &parsing);
			else if (*fmt == 'd')
				i += convert_d(ap, &parsing);
			else if (*fmt == 'x')
				i += convert_x(ap, &parsing);
			fmt++;
		}
		else
		{
			ft_putchar(*fmt);
			fmt++;
			i++;
		}
	}
	va_end(ap);
	return (i);
}
