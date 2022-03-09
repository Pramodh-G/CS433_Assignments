#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<assert.h>
#include<omp.h>

#define SIZE (100)
#define NTHREADS (1)

long double **L, *y, *x;
int n;
int nthreads;

void InitializeInput(long double** L, long double* x, long double* y)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            L[i][j] = ((long double)(random() % 100)) / 100.0 + 0.01;
        }
    }
    for (int i = 0; i < n; i++)
    {
        y[i] = ((long double)(random() % 100)) / 100.0 + 0.01;
    }
    for (int i = 0; i < n; i++)
    {
        x[i] = ((long double)(random() % 100)) / 100.0 + 0.01;
    }
}

int main(int argc, char *argv[])
{
    n = strtol(argv[1], NULL, 10);
    // n = SIZE;
    nthreads = strtol(argv[2], NULL, 10);
    // nthreads = NTHREADS;
    // Input taking!!
    L = (long double**)malloc((n)*sizeof(long double*) );
    assert(L != NULL);
    for (int i = 0; i < n; i++)
    {
        L[i] = (long double*)malloc((i + 1)*sizeof(long double));
        assert(L[i] != NULL);

        // for (int j = 0; j <= i; j++)
        // {
        //     scanf("%Lf", &L[i][j]);
        // }

    }
    // note size of long double 16 bytes. cache line in my computer is 64 bytes. to avoid false sharing, pad elements.
    y = (long double*)malloc(4 * (n) * sizeof(long double));
    x = (long double*)malloc(4 * (n) * sizeof(long double));
    assert(y != NULL);
    assert(x != NULL);
    InitializeInput(L, x, y);
    // for (int i = 0; i < n; i++)
    // {
    //     scanf("%Lf", &y[4 * i]);
    //     x[4 * i] = ((long double)(random() % 100)) / 100;
    // }

    // Solving
    double start = 0.0, end = 0.0;
    start = omp_get_wtime();
    for (int j = 0; j < n; j++)
    {
        #pragma omp single
        {
            x[4 * j] = y[4 * j] / L[j][j];
            // printf("i: %d y_i: %lf, x_i: %lf, L[i][i]: %lf\n", j, x[4 * j], y[4 * j], L[j][j]);
        }

        #pragma omp parallel for num_threads(nthreads)
        for (int i = j + 1; i < n; i++)
        {
            // printf("in thread no %d\n", omp_get_thread_num());
            y[i * 4] -= (x[4 * j] * L[i][j]);
        }
    }
    end = omp_get_wtime();
    printf("Solution took %lf seconds--------------------\n", end - start);
    // for (int i = 0; i < n; i++)
    // {
    //     printf("%f ", x[4 * i]);
    // }
    return 0;
}