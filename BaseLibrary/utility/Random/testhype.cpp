/*************************** TESTHYPE.CPP ******************* AgF 2001-11-15 *
*                                                                            *
*  Test hypergeometric distribution                                          *
*  Necessary files are in stocc.zip                                          *
*                                                                            *
*****************************************************************************/

#include <time.h>                      // define time()
#include "randomc.h"                   // define classes for random number generators
#include "mersenne.cpp"                // code for random number generator
#define RANDOM_GENERATOR TRandomMersenne // define which random number generator to use
#include "stocc.h"                     // define random library classes
#include "stoc1.cpp"                   // random library source code
#include "stoc2.cpp"                   // random library source code
#include "userintf.cpp"                // define system specific user interface


// main  
int main() {
  // parameters. You may change these
  int32 n = 8;                         // number of balls taken
  int32 m = 20;                        // number of red balls
  int32 N = 50;                        // total number of balls
  int32 nn = 1000000;                  // number of samples
  
  // other variables
  double sum;                          // sum
  double ssum;                         // squaresum
  int32 min, max;                      // minimum, maximum
  double mean;                         // mean
  double var;                          // variance
  int32 i;                             // loop counter
  int32 x;                             // random variate
  const int DSIZE = 18;                // size of array
  int32 dis[DSIZE] = {0};              // number of samples in each range
  int c;                               // index into dis list
  double f;                            // calculated function value
  double xme;                          // expected mean
  double xva;                          // expected variance
  double xsd;                          // expected standard deviation
  int32 delta;                         // step in list
  int32 xa, xb;                        // list minimum, maximum
  int32 x1, x2;                        // category range
  
  // make random seed
  int32 seed = time(0);                // random seed = time

  // make instance of random library for producing random variates
  StochasticLib1 sto(seed);            // object of class StochasticLib1

  // calculate exact mean and variance
  xme = (double)n*m/N;                 // calculated mean
  xva = xme*(1.-(double)m/N)*(double)(N-n)/(N-1.);  // calculated variance

  // calculate appropriate list divisions
  xsd = sqrt(xva);                     // calculated std.dev.
  xa = int(xme - 6.*xsd + 0.5);        // calculated minimum
  if (xa < 0) xa=0;
  if (xa < n+m-N) xa = n+m-N;  
  delta = int(12.*xsd/(DSIZE-1));      // list step
  if (delta < 1) delta = 1; 
  xb = xa + (DSIZE-1)*delta;           // calculated maximum
  if (xb > n) xb = n;
  if (xb > m) xb = m;

  // initialize
  sum = ssum = 0; min = 2000000000; max = -1;

  // start message
  printf("taking %li samples from hypergeometric distribution...", nn);
  
  // sample nn times
  for (i = 0; i < nn; i++) {

    // generate random number with desired distribution
    x = sto.Hypergeometric(n,m,N);

    // update sums
    sum += x;
    ssum += (double)x*x;
    if (x < min) min = x;
    if (x > max) max = x;
    c = (x-xa)/delta;
    if (c < 0) c = 0;
    if (c >= DSIZE) c = DSIZE-1;
    dis[c]++;}
    
  // calculate sampled mean and variance
  mean = sum / nn; 
  var = (ssum - sum*sum/nn) / nn;

  // print sampled and theoretical mean and variance
  printf("\n\nparameters: n=%li, m=%li, N=%li", n, m, N);
  printf("\n                mean        variance");
  printf("\nsampled:   %12.6f %12.6f", mean, var);
  printf("\nexpected:  %12.6f %12.6f", xme,  xva);

  // print found and expected frequencies
  printf("\n\n      x              found     expected");
  for (c = 0; c < DSIZE; c++) {
    x1 = c*delta + xa;
    if (x1 > xb) break;
    x2 = x1+delta-1;
    if (x2 > xb) x2 = xb;
    if (c==0 && min<x1) x1 = min;
    if (c==DSIZE-1 && max>x2) x2 = max;

    // calculate expected frequency
    for (x=x1, f=0.; x <= x2; x++) {
      f += exp(LnFac(m) - LnFac(x) - LnFac(m-x) + LnFac(N-m) -
        LnFac(n-x) - LnFac(N-m-n+x) - (LnFac(N) - LnFac(n) - LnFac(N-n)));}

    // output found and expected frequency
    if (dis[c] || f*nn > 1E-4) {
      if (x1==x2) {
        printf("\n%7li       %12.6f %12.6f", x1, (double)dis[c]/nn, f);}
      else {
        printf("\n%6li-%-6li %12.6f %12.6f", x1, x2, (double)dis[c]/nn, f);}}}


  EndOfProgram();                      // system-specific exit code
  return 0;}

