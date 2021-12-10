#include "ai.h"
#include <iostream>
#include <time.h>

using namespace std;

float randomf(float lower, float upper)
{
    return ((float)(rand() % (int)1e6) / 1e6) * (upper - lower) + lower;
}

void init_values(genes* g, values* v)
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
    for(int i = 0; i < 8; i++)
    {
        v->x[i] = 0;
        v->a1[i] = 0;
        v->a2[i] = 0;
        if(i < 3) v->y[i] = 0;
    }
}

void mutate_values(genes* g, genes prev)
{
    for(int i = 0; i < 8; i++)
    {
        g->b1[i] = prev.b1[i] + randomf(-0.1, 0.1);
        g->b2[i] = prev.b2[i] + randomf(-0.1, 0.1);
        if(i < 3) g->by[i] = prev.by[i] + randomf(-0.1, 0.1);
        for(int j = 0; j < 8; j++)
        {
            g->w1[i][j] = prev.w1[i][j] + randomf(-0.01, 0.01);
            g->w2[i][j] = prev.w2[i][j] + randomf(-0.01, 0.01);
            if(j < 3) g->wy[i][j] = prev.wy[i][j] + randomf(-0.01, 0.01);
        }
    }
}

int feed_forward(genes* g, values* v)
{
    if(g == NULL || v == NULL)
        return rand() % 3 + 1;

    vector_matrix(v->a1, v->x, g->w1[0], 8, 8);
    vector_plus_vector(g->b1, v->a1, 8);
    ReLU(v->a1, 8);

    vector_matrix(v->a2, v->a1, g->w2[0], 8, 8);
    vector_plus_vector(g->b2, v->a2, 8);
    ReLU(v->a1, 8);

    vector_matrix(v->y, v->a2, g->wy[0], 8, 3);
    vector_plus_vector(g->b1, v->y, 3);
    ReLU(v->a1, 3);

    int maks = 0;
    for(int i = 0; i < 3; i++)
    {
        if(v->y[i] > v->y[maks])
            maks = i;
    }
    return maks;
}
