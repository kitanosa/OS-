#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "./lib/imp_0/sinintegral.h"
#include "./lib/imp_0/e.h"

int main()
{
	char cmd = '0';
	int x;
	float a = 0.0;
	float b = 0.0;
	float c = 0.0;
	printf("Enter command:\n");
	printf("1 arg1 arg2 arg3 -> return value - float; arg1,arg2,arg3 - float\n");
	printf("2 arg1 -> return value - float; arg1 - int\n");
	cmd = getchar();
	if(cmd == '1')
	{
		if(scanf("%f%f%f", &a, &b, &c) != 3)
		{
			printf("INVALID INPUT\n");
			exit(-1);
		}
		printf("integral = %f\n", sinintegral(a, b, c));
	}
	else if (cmd == '2')
	{
		if(scanf("%d", &x) != 1)
		{
			printf("INVALID INPUT\n");
			exit(-1);
		}
		printf("E = %f\n", E(x));
	}
	else
	{
		printf("INVALID COMMAND\n");
		exit(-1);
	}
}