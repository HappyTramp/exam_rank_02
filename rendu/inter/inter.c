/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iter.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exam <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 17:00:51 by exam              #+#    #+#             */
/*   Updated: 2020/02/14 17:13:39 by exam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int		ft_strlen(char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

int	in_str(char *s, char c, int n)
{
	while (*s && n-- > 0)
		if (c == *s++)
			return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	char *s1;
	char *s2;
	int		i;

	if (argc != 3)
	{
		write(1, "\n", 1);
		return (1);
	}
	s1 = argv[1];
	s2 = argv[2];
	i = 0;
	while (s1[i])
	{
		if (!in_str(s1, s1[i], i) && in_str(s2, s1[i], ft_strlen(s2)))
			ft_putchar(s1[i]);
		i++;
	}
	ft_putchar('\n');
	return (0);
}
