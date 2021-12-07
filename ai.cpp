#include "ai.h"
#include <iostream>
#include <time.h>

using namespace std;

float x[8];
float a1[8];
float a2[8];
float y[3];


float randomf(float lower, float upper)
{
    return ((float)(rand() % (int)1e6) / 1e6) * (upper - lower) + lower;
}

void init_values(genes* g)
{
    for(int i = 0; i < 8; i++)
    {
        g->b1[i] = randomf(-10, 10);
        g->b2[i] = randomf(-10, 10);
        if(i < 3) g->by[i] = randomf(-10, 10);
        for(int j = 0; j < 8; j++)
        {
            g->w1[i][j] = randomf(-1, 1);
            g->w2[i][j] = randomf(-1, 1);
            if(j < 3) g->wy[i][j] = randomf(-1, 1);
        }
    }
}

int feed_forward()
{
    return rand() % 3 + 1; // ima neki veoma cudnovati bag gde uleti u sebe smekerski
}
