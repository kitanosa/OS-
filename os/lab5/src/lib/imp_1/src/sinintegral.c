#include <stdio.h>
#include <math.h>
#include "../sinintegral.h"

float sinintegral(float a, float b, float e) 
{ //              		а-нпи    b-впи    e-шаг
	int n = (b-a)/e; //шаг (разбиение отрезка на n равных отрезков)
	float integral = 0.0;
	float c = 0.0;
	if (n < 0) { n = -n; }
	if(a < b) { c = a; }
	else { c = b; }
	for(int i = 0; i < n; ++i)
	{
		integral += (sinf(c)+sinf(c+e))/2.0*e;
		c += e;
	}
	printf("2 реализация методом трапеции\n");
	return integral;
}