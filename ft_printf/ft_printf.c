#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct
{
	int precision;
	int width;
	char specifier;
}	t_format_spec;

void ft_putchar(char c)
{
	write(STDOUT_FILENO, &c, 1);
}

int ft_strlen(char *s)
{
	int c;

	c = 0;
	while (s[c])
		c++;
	return (c);
}

void ft_putstr(char *s)
{
	write(STDOUT_FILENO, s, ft_strlen(s));
}

int ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

int ft_atoi_skip(char **s)
{
	int nb;
	int is_neg;

	is_neg = **s == '-';
	nb = 0;
	while (ft_isdigit(**s))
	{
		nb *= 10;
		nb += **s - '0';
		(*s)++;
	}
	return (is_neg ? -nb : nb);
}

char *parse(char *format, t_format_spec *spec)
{
	int tmp;

	if (ft_isdigit(*format))
		tmp = ft_atoi_skip(&format);
	if (*format == '.')
	{
		spec->precision = tmp;
		if (ft_isdigit(*++format))
			spec->width = ft_atoi_skip(&format);
	}
	else
		spec->width = tmp;
	if (*format != 's' && *format != 'd' && *format != 'x')
		return (NULL);
	spec->specifier = *format++;
	return (format);
}

int count_digits(int nb, int radix)
{
	int counter;

	counter = nb < 0 ? 1 : 0;
	while (nb > 0)
	{
		nb /= radix;
		counter++;
	}
	return (counter);
}

char *ft_itoa_base(int nb, char *base)
{
	int len;
	int radix;
	char *s;

	radix = ft_strlen(base);
	len = count_digits(nb, radix);
	if ((s = (char*)malloc(sizeof(char) * (len + 1))) == NULL)
		return (NULL);
	s[len] = 0;
	if (nb < 0)
		s[0] = '-';
	while (--len >= (nb < 0 ? 1 : 0))
	{
		s[len] = base[nb % radix];
		nb /= radix;
	}
	return (s);
}

int handle_s(va_list ap, t_format_spec *spec)
{
	char *tmp;

	tmp = va_arg(ap, char*);
	ft_putstr(tmp);
	return (ft_strlen(tmp));
}

int handle_d(va_list ap, t_format_spec *spec)
{
	int tmp;
	char *s;

	tmp = va_arg(ap, int);
	s = ft_itoa_base(tmp, "0123456789");
	ft_putstr(s);
	return ft_strlen(s);
}

int handle_x(va_list ap, t_format_spec *spec)
{
	int tmp;
	char *s;

	tmp = va_arg(ap, int);
	s = ft_itoa_base(tmp, "0123456789abcdef");
	ft_putstr(s);
	return ft_strlen(s);
}

int print_spec(va_list ap, t_format_spec *spec)
{
	if (spec->specifier == 's')
		return (handle_s(ap, spec));
	else if (spec->specifier == 'd')
		return (handle_d(ap, spec));
	else if (spec->specifier == 'x')
		return (handle_x(ap, spec));
	else
		return (-1);
}

int ft_printf(char *format, ...)
{
	va_list ap;
	int counter;
	t_format_spec spec;

	va_start(ap, format);


	counter = 0;
	while (*format)
	{
		if (*format == '%')
		{
			if ((format = parse(format + 1, &spec)) == NULL)
				return (-1);
			counter += print_spec(ap, &spec);
		}
		else
		{
			ft_putchar(*format++);
			counter++;
		}
	}
	va_end(ap);
	return (counter);
}
