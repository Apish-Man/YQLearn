#include <stdio.h>

void test1(char* str)
{
	printf("test1:%p\n",&str);
}

int main()
{
	char *arr="orange";
	printf("arr:%p\n",arr);
	test1(arr);
	return 0;
}
