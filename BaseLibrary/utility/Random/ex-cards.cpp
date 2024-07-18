/************************** EX-CARDS.CPP ******************** AgF 2001-11-11 *
*                                                                            *
* Example of using non-uniform random variate generator library to shuffle   *
* a list of numbers.                                                         *
*                                                                            *
* This example shuffles a deck of cards.                                     *
*                                                                            *
* Compile for console mode.                                                  *
*****************************************************************************/

#include <time.h>                      // define time()
#include "randomc.h"                   // define random number generator classes
// define which random number generator to base random library on:
#define RANDOM_GENERATOR TRandomMersenne
#include "stocc.h"                     // define random library classes
#include "mersenne.cpp"                // random number generator source code
#include "stoc1.cpp"                   // random library source code
#include "userintf.cpp"                // define system specific user interface

int main () {
  int32 seed = time(0);                // generate random seed
  StochasticLib1 sto(seed);            // make instance of random library
  int deck[52];                        // deck of 52 cards
  char * ColorNames[] = {              // names of 4 colors
  "clubs", "diamonds", "hearts", "spades"};
  char * ValueNames[] = {              // names of 13 card values
  "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
  int i;                               // loop counter
  int color;                           // card color
  int value;                           // card value

  // Make shuffled list of cards. 
  // The 52 cards are numbered from 0 to 51, where 0 = A-clubs, 1 = A-diamonds,
  // 51 = K-spades:
  sto.Shuffle(deck, 0, 52);

  // output heading text
  printf("Shuffled deck of cards:\n\n");
  
  // loop for all cards
  for (i=0; i<52; i++) {
    // translate card number into color and value
    color = deck[i] % 4;
    value = deck[i] / 4;

    // output card
    printf("%8s %2s     ", ColorNames[color], ValueNames[value]);
    // make linefeed for every four cards
    if (i % 4 == 3) printf("\n");}
     
  EndOfProgram();                      // system-specific exit code
  return 0;}