#include <stdio.h>

int main(void)
{
	// int sum[4];
	// for (int i = 0; i<4; i++)
	// {
	// 	scanf("%i", &sum[i]);
	// 	// printf("%i\n", sum[i]);
	// 	// printf("%p\n", &sum[i]);
	// 	// printf("%p\n", *(sum[i]));
	// 	printf("%i\n", *(sum + i));	//*(sum + i)
	// }  

	// // int num;
	// // scanf("%i", &(num));
	// // printf("%i\n", *&num);

	// char *s = "Emma";
	// printf("%p\n", s);
	// printf("%c \n", *s);
	// printf("%s\n", s);
	// printf("%p \n", &(*s));

	// char *s;
	// printf("%p\n", s);

	int i = 9;
	// int *idx = &i;
	int *idy = i;
	// printf("%p\n", *idx);
	printf("%p\n", &idy);
	printf("%i\n", idy);
}