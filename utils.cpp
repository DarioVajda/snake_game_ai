#include "ai.h"
#include <iostream>

using namespace std;

void vector_plus_vector(float *s, float *v, int n)
{
    for(int i = 0; i < n; i++)
        *(v+i) += *(s+i);
}

void scalar_matrix(float s, float *matrix, int m, int n)
{
    for(int i = 0; i < m; i++)
        for(int j = 0; j < n; j++)
            *(matrix+i*n + j) *= s;
}

void vector_matrix(float *rez, float *a, float *b, int m, int n)
{
    for(int i = 0; i < n; i++)
    {
        *(rez + i) = 0;
        for(int j = 0; j < m; j++)
        {
            *(rez + i) += *(a+j) * (*(b+i*n+j));
        }
    }
}

void ReLU(float* a, int n)
{
    for(int i = 0; i < n; i++)
    {
        if(*(a+i) < 0)
        {
            *(a+i) = 0;
        }
    }
}
