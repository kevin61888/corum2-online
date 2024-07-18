/*************************** EX-STOC.CPP ******************** AgF 2001-11-11 *
*                                                                            *
* Example showing how to use non-uniform random variate generator library.   *
* Necessary files are in stocc.zip.                                          *
*                                                                            *
* Compile for console mode.                                                  *               *
* © 2001 Agner Fog. GNU General Public License www.gnu.org/copyleft/gpl.html *
*****************************************************************************/

#include <time.h>                      // define time()
#include "randomc.h"                   // define classes for random number generators
#include "userintf.cpp"                // define system specific user interface

// define which random number generator to base random library on:
#include "mersenne.cpp"                // members of class TRandomMersenne
#define RANDOM_GENERATOR TRandomMersenne
// or:
//#include "ranrotw.cpp"               // members of class TRanrotWGenerator
//#define RANDOM_GENERATOR TRanrotWGenerator  // or:
// or:
//#include "mother.cpp"                // members of class TRanrotWGenerator
//#define RANDOM_GENERATOR TRandomMotherOfAll

#include "stocc.h"                     // define random library classes
#include "stoc1.cpp"                   // random library source code

int main() {
  int32 seed = time(0);                // random seed
  StochasticLib1 sto(seed);            // make instance of random library
  int i;                               // loop counter
  double r;                            // random number
  int ir;                              // random integer number

  // make random numbers with uniform distribution
  printf("Random numbers with uniform distribution:\n");
  for (i=0; i<16; i++) {
    ir = sto.IRandom(0, 20);
    printf("%8i  ", ir);}

  // make random numbers with normal distribution
  printf("\n\nRandom numbers with normal distribution:\n");
  for (i=0; i<16; i++) {
    r = sto.Normal(10, 4);
    printf("%8.5f  ", r);}

  // make random numbers with poisson distribution
  printf("\n\nRandom numbers with poisson distribution:\n");
  for (i=0; i<16; i++) {
    ir = sto.Poisson(10);
    printf("%8i  ", ir);}
  
  // make random numbers with binomial distribution
  printf("\n\nRandom numbers with binomial distribution:\n");
  for (i=0; i<16; i++) {
    ir = sto.Binomial(40, 0.25);
    printf("%8i  ", ir);}
  
  // make random numbers with hypergeometric distribution
  printf("\n\nRandom numbers with hypergeometric distribution:\n");
  for (i=0; i<16; i++) {
    ir = sto.Hypergeometric(20, 20, 40);
    printf("%8i  ", ir);}

  
  EndOfProgram();                      // system-specific exit code
  return 0;}

