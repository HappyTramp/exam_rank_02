/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exam <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 17:00:41 by exam              #+#    #+#             */
/*   Updated: 2020/02/24 17:08:12 by exam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int ft_strlen(char *s)
{
	int i;
	
	i = 0;
	while (s[i])
		i++;
	return (i);
}

int in_str(char *s, char c, int n)
{
	while (*s && n--)
		if (*s++ == c)
			return (1);
	return (0);
}

int main(int argc, char **argv)
{
	char	*s1;
	char	*s2;
	int		i;
	
	if (argc != 3)
	{
		write(1, "\n", 1);
		return (0);
	}
	i = 0;
	s1 = argv[1];
	s2 = argv[2];
	while (s1[i])
	{
		if (!in_str(s1, s1[i], i) && in_str(s2, s1[i], ft_strlen(s2)))
			write(1, s1 + i, 1);
		i++;
	}
	write(1, "\n", 1);
	return (0);
}
