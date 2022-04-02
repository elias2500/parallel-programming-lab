#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <emmintrin.h>

void get_walltime(double *wct) {
  struct timeval tp;
  gettimeofday(&tp,NULL);
  *wct = (double)(tp.tv_sec+tp.tv_usec/1000000.0);
}

int main()
{
  double ts, te;

  float *a, *b;

  const __m128 k = _mm_set1_ps(0.5);
  const __m128 k1 = _mm_set1_ps(5.0);

  const float c = 0.5;
  const float c1 = 5.0;

  __m128 sum;

  int i;
  int br = 0;

  i = posix_memalign((void **)&a,16,N*M*sizeof(float));
  if (i!=0) exit(1);

  i = posix_memalign((void **)&b,16,N*M*sizeof(float));
  if (i!=0) exit(1);


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
      sum = _mm_set1_ps(0);
      
      sum = _mm_add_ps(_mm_mul_ps(_mm_loadu_ps(&a[i+j]),k1), sum);
      sum = _mm_add_ps(_mm_mul_ps(_mm_loadu_ps(&a[i+(j-1)]),k), sum);
      sum = _mm_add_ps(_mm_mul_ps(_mm_loadu_ps(&a[i+(j+1)]),k), sum);
      sum = _mm_add_ps(_mm_mul_ps(_mm_loadu_ps(&a[(i-1)+j]), k), sum);
      sum = _mm_add_ps(_mm_mul_ps(_mm_loadu_ps(&a[(i-1)+(j-1)]), k), sum);
      sum = _mm_add_ps(_mm_mul_ps(_mm_loadu_ps(&a[(i-1)+(j+1)]), k), sum);
      sum = _mm_add_ps(_mm_mul_ps(_mm_loadu_ps(&a[(i+1)+j]), k), sum);
      sum = _mm_add_ps(_mm_mul_ps(_mm_loadu_ps(&a[(i+1)+(j-1)]), k), sum);
      sum = _mm_add_ps(_mm_mul_ps(_mm_loadu_ps(&a[(i+1)+(j+1)]), k), sum);

      _mm_storeu_ps(&b[i+j],sum);
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
            if(b[i*N+j] != (((a[(i-1)*N+(j-1)])*c)+((a[i*N+(j-1)])*c)+((a[(i+1)*N+(j-1)])*c)+((a[i*N+j])*c1)+((a[(i-1)*N+j])*c)+((a[(i+1)*N+j])*c)+((a[(i-1)*N+(j+1)])*c)+((a[i*N+(j+1)])*c)+((a[(i+1)*N+(j+1)])*c)))
            {
                printf("Error in calculation!\n");
                br = 1;
                break; 
            }
        }
        if (br==1)
        {
            break;
        }
    }


  free(a);
  free(b);

  return 0;
}

