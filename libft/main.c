#include "libft.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	argc = argc + 0;
	printf("ft_isdigit : %d, input %c\n", ft_isdigit('*'), '*');
	printf("ft_atoi : %d, input %s\n", ft_atoi("27670116110564327421"), "27670116110564327421");
	printf("atoi : %d, input %s\n", atoi("27670116110564327421"), "27670116110564327421");
	printf("ft_strdup : %s, input %s\n", ft_strdup(argv[1]), argv[1]);
	printf("ft_strlen : %zd, input %s\n", ft_strlen(argv[1]), argv[1]);
	printf("ft_isalpha : %d, input %c\n", ft_isalpha('*'), '*');
	printf("ft_toupper : %c, input %c\n", ft_toupper('d'), 'd');
	printf("ft_tolower : %c, input %c\n", ft_tolower('X'), 'X');
	printf("ft_isprint : %d, input %c\n", ft_isprint(2), 2);
	printf("ft_isalnum : %d, input %c\n", ft_isalnum('9'), '9');
	printf("ft_isascii : %d, input %c\n", ft_isascii('9'), '9');
	
	char	*str_base;
	char	dest[100];
	char	*src;
	int		index;

	str_base = "Hello";
	src = " World";
	index = 0;
	while (index < 6)
	{
		dest[index] = str_base[index];
		index++;
	}
	printf("ft_strlcat : (%zd) $%s$\n", ft_strlcat(dest, src, 3), dest);
	printf("ft_strlcpy : (%zd) $%s$\n", ft_strlcpy(dest, src, 10), dest);
	printf("ft_strncmp : (%d) $%s$\n", ft_strncmp(dest, "Hello", 10), dest);
	printf("ft_strchr : (%s) \n", ft_strchr(dest, '\0'));
	printf("ft_strrchr : (%s)\n", ft_strrchr("923456123", '\0'));
	ft_memset(dest, '9', 10);
	printf("ft_memset : (%s) $%c$\n", dest, '9');
	char str[10] = "123456789";
	ft_bzero(str, 5);
	printf("ft_bzero : (");
	for(int i=0;i<10;i++)
	{
		printf("%c", str[i]);
	}
	printf(")\n");
	char *a = "123";
	a = ft_memchr(a, '3', 4);
	printf("ft_memchr : (%s)\n", a);
	printf("%s\n", dest);
	ft_memcpy(dest, src, sizeof(dest));
	printf("ft_memcpy : (%s)\n", dest);
	printf("%s\n", dest);
	ft_memmove(dest, "1423", sizeof(dest));
	printf("ft_memcpy : (%s)\n", dest);
	printf("ft_memcmp : (%d) $%s$\n", ft_memcmp(dest, "hello", 10), dest);
	printf("ft_strnstr : (%s) $%s$\n", ft_strnstr("1234243", "43", 6), dest);
	printf("ft_substr : (%s) $%s$\n", ft_substr("126", 10, 6), dest);
	printf("ft_strjoin : (%s)\n", ft_strjoin("126", "1234"));
	printf("ft_strtrim : (%s)\n", ft_strtrim("lorem \n ipsum \t dolor \n sit \t amet", "\t \n"));
	printf("ft_itoa : (%s)\n", ft_itoa(0));
	ft_putchar_fd('c', 1);
	printf("\n");
	ft_putstr_fd("hello world", 1);
	printf("\n");
	ft_putendl_fd("hello world", 1);
	ft_putnbr_fd(-2147483648, 1);
	printf("\n");
	
	return 0;
}
