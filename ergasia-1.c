#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void get_walltime(double *wct) {
  struct timeval tp;
  gettimeofday(&tp,NULL);
  *wct = (double)(tp.tv_sec+tp.tv_usec/1000000.0);
}

int main()
{
    double ts, te;

    float *a, *b;

    int br = 0;

    const float k = 0.5;
    const float k4 = 5.0;

    a = (float *)malloc(N*M*sizeof(float));
    if (a==NULL) 
    {
        exit(1);
    }

    b = (float *)malloc(N*M*sizeof(float));
    if (b==NULL) 
    {
        exit(1);
    }

    for (int i=0;i<N*M;i++) 
    {
        a[i] = rand()%10+1;
    }

    for (int i=0;i<N*M;i++) 
    {
        b[i] = rand()%10+1;
    }

    // get starting time (double, seconds) 
    get_walltime(&ts);

    for(int i=1;i<M-1;i++)
    {
        for(int j=1;j<N-1;j++)
        {
            b[i*N+j] = ((a[((i-1)*N)+(j-1)])*k)+
            ((a[(i*N)+(j-1)])*k)+
            ((a[((i+1)*N)+(j-1)])*k)+
            ((a[(i*N)+j])*k4)+
            ((a[((i-1)*N)+j])*k)+
            ((a[((i+1)*N)+j])*k)+
            ((a[((i-1)*N)+(j+1)])*k)+
            ((a[(i*N)+(j+1)])*k)+
            ((a[((i+1)*N)+(j+1)])*k); 
        }
    }

    // get ending time
    get_walltime(&te);

    // print computation time
    printf("Computation time = %f sec\n",(te-ts));

    for(int i=1;i<M-1;i++)
    {
        for(int j=1;j<N-1;j++)
        {
            if(b[i*N+j] != (((a[(i-1)*N+(j-1)])*k)+((a[i*N+(j-1)])*k)+((a[(i+1)*N+(j-1)])*k)+((a[i*N+j])*k4)+((a[(i-1)*N+j])*k)+((a[(i+1)*N+j])*k)+((a[(i-1)*N+(j+1)])*k)+((a[i*N+(j+1)])*k)+((a[(i+1)*N+(j+1)])*k)))
            {
                printf("Error!\n");
                br = 1;
                break; 
            }
        }
        if (br==1)
        {
            break;
        }
    }

    free(b);
    free(a);

    return 0;
}