#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#define MAXVAL 20


double f1(double x) {
    return 2.17 * log(x) * cos(x); // not O(1), O is polynomial
}

double f(double x) {
    return 1.17 * x * x;
}
//HTOP see
int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    int p = atoi(argv[2]);
    printf("n = %d\n",n);
    printf("p = %d\n",p);

    double *tt = malloc(sizeof (double) * n);
    srand(getpid());

    for(int i =0; i<n; i++) {
        tt[i] = rand() % MAXVAL;
    }


    clock_t start, end;
    double cpu_time_used_seq;
    start = clock();
    //seq
    int i =0;
    for(i =0; i<n; i++) {
        //tt[i] = f(tt[i]);
        tt[i] = f1(tt[i]);
    }
    end = clock();
    cpu_time_used_seq = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("seq: for n=%d, time taken=%f\n",n,cpu_time_used_seq);
    //end seq

    for (int pp = 2; pp<17; pp++){
        p = pp;
        int chunk = n/p; //acts like floor
        omp_set_num_threads(p);
        //start parallel
        double start_parallel = omp_get_wtime();
        #pragma parallel shared(tt,chunk) private (i)
        {
            #pragma omp for schedule(dynamic,chunk) nowait
                for(i=0;i<n;i++) {
                    //tt[i] = f(tt[i]);
                    tt[i] = f1(tt[i]); //the speed up is much higher with f1 since log and cos takes time to be computed
                }
            printf("%d\n",omp_get_num_threads());
        }
        double end_parallel = omp_get_wtime();
        double cpu_time_used_parallel = (double) (end_parallel - start_parallel);
        printf("parallel: for n=%d, p=%d, time taken=%f, speedup=%f\n",n,omp_get_num_threads(),cpu_time_used_parallel,cpu_time_used_seq/cpu_time_used_parallel);
        //end parralel
    }
}
