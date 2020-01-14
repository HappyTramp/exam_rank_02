int ft_printf(char *format, ...);

int main(void)
{
	ft_printf("bonjour\n");
	ft_printf("bon\n\0jour\n");

	ft_printf("s: %s\n", "yoyo");
	ft_printf("d: %d\n", 123);
	ft_printf("x: %x\n", 0xffee);
}
