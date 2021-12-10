#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED

typedef struct Genes
{
    float w1[8][8];
    float w2[8][8];
    float wy[8][3];

    float b1[8];
    float b2[8];
    float by[3];
} genes;

typedef struct Values
{
    float x[8];
    float a1[8];
    float a2[8];
    float y[3];
} values;

float randomf(float lower, float upper);
int feed_forward(genes* g, values* v);
void init_values(genes* g, values* v);
void vector_plus_vector(float *s, float* v, int n);
void vector_matrix(float* rez, float *a, float *b, int m, int n);
void ReLU(float* a, int n);
void mutate_values(genes* g, genes prev);

#endif // AI_H_INCLUDED
