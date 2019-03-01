// This is my code
// Jen Moutenot
// CS366

// model         : 63
// machine name  : Intel(R) Xeon(R) CPU E5-2620 v3 @ 2.40GHz
// cache size    : 15360
// alignment     : 64
// L1d cache     : 32K
// L1i cache     : 32K
// L2 cache      : 256K
// L3 cache      : 15360K
// hostname      : snoopy

#include <stdio.h>
#include <stdlib.h>
#include "mm.h"
#include "fcycmm.h"
#include "clock.h"

/* whether or not fcyc should clear the cache */
#define CLEARCACHE 1  

/* global arrays [ ok for this kind of hacking, but not in production ] */
array ga, gb, gc;

/* purpose: check the result array for correctness 
 * input: the product array, c, and its size, n
 * returns: nothing (complains in an error message if result is wrong)
 */
void checkresult(array c, int n)
{
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      if (c[i][j] != (double)n) 
      {
        printf("Error: bad number (%f) in result matrix (%d,%d)\n", 
            c[i][j], i, j);
        fflush(stdout);
        exit(0);
      }
}


/* purpose: Run function f and return clocks per inner loop iteration 
 * input: a function, f, and the matrix size, n
 * returns: the number of cycles taken per inner loop iteration
 */
double run(test_funct f, int n)
{
  double cpi;

  cpi = fcyc(f, n, CLEARCACHE) / (n*n*n);
  checkresult(gc, n);
  return cpi;
}













/* purpose: reset result array to zero 
 * input: the array, c, and its size, n
 * returns: the array updated to all zeros.
 */
void reset(array c, int n)
{
  int i,j;
  
  for (i = 0; i < n; i++) 
    for (j = 0; j < n; j++) 
      c[i][j] = 0.0;
}


/* purpose: initialize input arrays to 1 
 * input: the input arrays, a and b, and their size, n
 * returns: the arrays updated to all ones.
 */
void init(array a, array b, int n) 
{
  int i,j;
  
  for (i = 0; i < n; i++) 
  {
    for (j = 0; j < n; j++) 
    {
      a[i][j] = 1.0;
      b[i][j] = 1.0;
    }
  }
}


/* purpose: print an array 
 * input:   an array, a, and its size, n
 * returns: nothing (printing is a side effect)
 */
void printarray(array a, int n)
{
  int i, j;

  for (i = 0; i < n; i++) 
  {
    for (j = 0; j < n; j++) 
      printf("%5.1f ", a[i][j]);
    
    printf("\n");
  }
}


















/* purpose: Original trace function determines product matrix 
 * input: array A, array B, array C, and their size n
 * returns: product matrix of array A and B
 */
void multiply(array A, array B, array C, int n) 
{
    int r, c, k;
    // for each row r = Monday then Tuesday then Wednesday 
    for (r = 0 ; r < n; ++r)
    {
        // for each column c = Sandy then Squidward
        for ( c = 0 ; c < n ; ++c)
        {
            // for each index k = 1 then 2 then 3
            for (k = 0; k < n ; ++k)
            //product[r,c] += usage[r,k] * costs[k,c]
            C[r][c] += A[r][k] * B[k][c];
        }
     }
}

/* purpose: Patrick function determines product matrix
 * input: array A, array B, array C, and their size n
 * returns: product matrix of array A and B
 */
void patrick(array A, array B, array C, int n)
{
     int c, r, k;
     // for each column c = Sandy then Squidward
     for (c = 0 ; c < n; ++c)
     {
         // for each row r = Monday then Tuesday then Wednesday
         for ( r = 0 ; r < n ; ++r) 
         { 
             // for each index k = 1 then 2 then 3
             for ( k = 0 ;  k < n ; ++k)
             //product[r,c] += usage[r,k] * costs[k,c]
             C[r][c] += A[r][k] * B[k][c];
         }
     }
}


/* purpose: Sponge function determines the product matrix
 * input: array A, array B, array C, and their size n
 * returns: product matrix of array A and B
 */
void sponge(array A, array B, array C, int n)
{
    int r, k, c;
    // for each row r = Monday then Tuesday
    for ( r = 0 ; r< n ; ++r)
    {
        // for each index k = 1 then 2 then 3
        for ( k = 0 ; k <n; ++k)
        {
            // for each column c = Sandy then Squidward
            for ( c = 0 ; c < n ; ++c)
            //product[r,c] += usage[r,k] * costs[k,c]
            C[r][c] += A[r][k] * B[k][c];
        }
    }
}






/* purpose: test the multiplcatoin of two 2x2 matricies 
 * input: nothing (the test is hard coded)
 * returns: nothing (prints result to standard out)
 */
void test()
{   
  ga[0][0] = 1;
  ga[0][1] = 2;
  ga[1][0] = 3;
  ga[1][1] = 4; 
  gb[0][0] = 5;
  gb[0][1] = 6;
  gb[1][0] = 7;
  gb[1][1] = 8;
  printf("product of \n");
  printarray(ga, 2); 
  printf("and\n"); 
  printarray(gb, 2);
  multiply(ga, gb, gc, 2);
  printf("=\n");
  printarray(gc, 2);
  
  if (gc[0][0] == 19 && gc[0][1] == 22 && gc[1][0] == 43 && gc[1][1] == 50)
    printf("Passed!!\n");
  else
    printf("Fail!!\n");
}   
      
      

/* 
 * Run matrix multiply and display performance
 * as clock cycles per inner loop iteration.
 */ 
int main()
{
  int n;

  init(ga, gb, MAXN);
  // test();
  // exit(0);

  printf("matmult cycles/loop iteration\n");
  fflush(stdout);
  for (n = MINN; n <= MAXN; n += INCN) 
  {  
    printf("%3d ", n);

    printf("%9.2f ", run(multiply, n));
    printf("\n");
    fflush(stdout);
  }

  exit(0);
}

