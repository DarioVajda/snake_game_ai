#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED

typedef struct xy
{
    float w1[8][8];
    float w2[8][8];
    float wy[8][3];

    float b1[8];
    float b2[8];
    float by[3];
} genes;

int feed_forward();
void init_values(genes* g);
void scalar_vector(float s, float* v, int n);
void scalar_matrix(float s, float* matrix, int m, int n);


#endif // AI_H_INCLUDED
