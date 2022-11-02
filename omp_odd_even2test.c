#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#ifdef DEBUG
const int RMAX = 100;
#else
const int RMAX = 10000000;
#endif

int thread_count;

void Usage(char* prog_name);

void Get_args(int argc, char* argv[]);
void Generate_list(int a[], int n);
void Odd_even(int a[], int n);

int main(int argc, char* argv[]) {
   FILE *file; 
   file = fopen("file.csv","w");
   int* a;
   
   double start, finish;
   Get_args(argc, argv);

  int n;
  for(int j = 1; j < 8; j++)
  {
    fprintf(file, "%i\t", j);
    n = pow(2, j);
    for(int i = 0; i < 10; i++)
    {
        a = malloc(n*sizeof(int));
        Generate_list(a, n);
        start = omp_get_wtime();
        Odd_even(a, n);
        finish = omp_get_wtime();
        fprintf(file, "%f\t", finish - start);
    }
    fprintf(file, "\n");
  }
  
  fclose ( file );

   free(a);
   return 0;
}  /* main */

void Usage(char* prog_name) {
   fprintf(stderr, "usage:   %s <thread count>, prog_name);
}  /* Usage */

void Get_args(int argc, char* argv[]) {
   if (argc != 4 ) {
      Usage(argv[0]);
      exit(0);
   }
   thread_count = strtol(argv[1], NULL, 10);
} 


/*-----------------------------------------------------------------
 * Function:  Generate_list
 * Purpose:   Use random number generator to generate list elements
 */
void Generate_list(int a[], int n) {
   int i;

   srandom(1);
   for (i = 0; i < n; i++)
      a[i] = random() % RMAX;
}

/*-----------------------------------------------------------------
 * Function:     Odd_even
 * Purpose:      Sort list using odd-even transposition sort
 * In args:      n
 * In/out args:  a
 */
void Odd_even(int a[], int n) {
   int phase, i, tmp;

#  pragma omp parallel num_threads(thread_count) \
      default(none) shared(a, n) private(i, tmp, phase)
   for (phase = 0; phase < n; phase++) {
      if (phase % 2 == 0)
#        pragma omp for 
         for (i = 1; i < n; i += 2) {
            if (a[i-1] > a[i]) {
               tmp = a[i-1];
               a[i-1] = a[i];
               a[i] = tmp;
            }
         }
      else
#        pragma omp for 
         for (i = 1; i < n-1; i += 2) {
            if (a[i] > a[i+1]) {
               tmp = a[i+1];
               a[i+1] = a[i];
               a[i] = tmp;
            }
         }
   }
}
