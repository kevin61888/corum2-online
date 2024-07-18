/*************************** TESTMWNC.CPP ******************* AgF 2002-01-04 *
*                                                                            *
*  Test multivariate Wallenius noncentral hypergeometric distribution        *
*  Necessary files are in stocc.zip                                          *
*                                                                            *
*                                                                            *
*  Instructions:                                                             *
*  1. Define your parameters below where indicated.                          *
*  2. Compile for console mode and run.                                      *
*  To test the univariate distribution set colors = 2                        *
*                                                                            *
*  The output will show:                                                     *
*  The measured mean and variance.                                           *
*  The theoretical mean calculated by the approximate formula.               *
*  The exact mean and variance are calculated only when possible within      *
*  a reasonable time.                                                        *
*****************************************************************************/

#include <time.h>                      // define time()
#include "randomc.h"                   // define classes for random number generators
#define RANDOM_GENERATOR TRandomMersenne // define which random number generator to use
#include "mersenne.cpp"                // code for this random number generator
#include "stocc.h"                     // define random library classes
#include "stoc1.cpp"                   // random library source code
#include "stoc3.cpp"                   // random library source code
#include "userintf.cpp"                // define system specific user interface


/*****************************************************************************
*     define your parameters here:                                           *
*****************************************************************************/

const int colors = 4;                  // number of colors

int32 n = 100;                         // number of balls to pick

int32 mlist[] = 
  {30, 60, 50, 20};                    // number of balls of each color

double wlist[] = 
  {1, 5, 3, 6};                        // weight of each color

int32 nsamp = 100000;                  // number of samples
    
 
/*****************************************************************************
*    main                                                                    *
*****************************************************************************/

int main() {

  int32 xlist[colors];                 // sampled vector
  double sum[colors];                  // sum of x
  double ssum[colors];                 // squaresum of x
  double mu1[colors];                  // calculated approximate mean
  double mu2[colors];                  // calculated exact mean
  double var[colors];                  // calculated exact variance
  int32 comb;                          // number of possible x combinations
  double mean1;                        // sampled mean
  double var1;                         // sampled variance
  double sumf;                         // sum of probabilities of all possible x
  double cmb;                          // product of elements in mlist
  int32 seconds;                       // measuring calculation time
  int32 x;                             // sample of color i
  int32 sam;                           // sample counter
  int i;                               // color index

  // make instance of random library with time as seed
  StochasticLib3 sto(time(0));

  // initialize arrays
  memset(sum, 0, sizeof(sum));
  memset(ssum, 0, sizeof(ssum));
  memset(mu2, 0, sizeof(mu2));
  memset(var, 0, sizeof(var));

  // print message
  printf("\nsampling from multivariate Wallenius noncentral hypergeometric distribution...");
  seconds = time(0);
  
  // sample nsamp times
  for (sam=0; sam < nsamp; sam++) {

    // make sample
    sto.MultiWalleniusNCHyp(xlist, mlist, wlist, n, colors);

    // update sums
    for (i=0; i<colors; i++) {
      x = xlist[i];
      sum[i] += x;
      ssum[i] += (double)x*x;}}

  // output time used for sampling
  seconds = time(0) - seconds;
  printf("\n%li samples in %lu s\n", nsamp, seconds);
    
  // calculate approximate mean
  CMultiWalleniusNCHypergeometricMoments mnc(n, mlist, wlist, colors, 1E-10f);
  mnc.mean(mu1);

  // estimate how much time exact calculation would take
  for (i=0, cmb=1.; i<colors; i++) if (mlist[i]) cmb *= mlist[i];
  if (cmb < 1E10) {
    // do exact calculation
    printf("\ncalculating all possible combinations...");
    seconds = time(0);
    sumf = mnc.moments(mu2,var, &comb);
    // output time used for exact calculation
    seconds = time(0) - seconds;
    printf("\n%li combinations calculated in %li s. sum-1 = %.3G\n", comb, seconds, sumf-1.);}

  // print table heading
  printf("\n                       sampled     approx      exact    sampled      exact");
  printf("\n   balls     weight       mean       mean       mean   variance   variance");
  
  // make table for all colors
  for (i=0; i<colors; i++) {
    // calculate sampled mean and variance
    mean1=sum[i]/nsamp;
    var1=(ssum[i]-sum[i]*sum[i]/nsamp)/(nsamp-1);
    // output table line
    printf("\n%8li %10.3f %10.3f %10.3f %10.3f %10.3f %10.3f",
    mlist[i], wlist[i], mean1, mu1[i], mu2[i], var1, var[i]);}


  EndOfProgram();                      // system-specific exit code
  return 0;}

