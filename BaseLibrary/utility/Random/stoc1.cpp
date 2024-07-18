/***************************** STOC1.CPP *********************** 2002-01-04 AF *
*
* Non-uniform random number generators.
*
* This file contains source code for the class StochasticLib1 defined in stocc.h.
*
* Documentation:
* ==============
* The file stocc.h contains class definitions.
* The file stocc.htm contains further instructions.
* The file distrib.pdf contains definitions of the statistic distributions.
* The file sampmet.pdf contains theoretical descriptions of the methods used
* for sampling from these distributions.
*
* ?2002 Agner Fog. GNU General Public License www.gnu.org/copyleft/gpl.html
*******************************************************************************/

#include <math.h>      // math functions
#include "stocc.h"     // class definition


/***********************************************************************
                     Log factorial function
***********************************************************************/
double LnFac(int32 n) {
  // log factorial function. gives natural logarithm of n!

  // define constants
  static const double        // coefficients in Stirling approximation     
    C0 =  0.918938533204672722,   // ln(sqrt(2*pi))
    C1 =  1./12., 
    C3 = -1./360.;
    // C5 =  1./1260.,  // use r^5 term if FAK_LEN < 50
    // C7 = -1./1680.;  // use r^7 term if FAK_LEN < 20
  // static variables
  static double fac_table[FAK_LEN]; // table of ln(n!):
  static int initialized = 0;   // remember if fac_table has been initialized

  if (n < FAK_LEN) {
    if (n <= 1) {
      if (n < 0) FatalError("Parameter negative in LnFac function");  
        exp((double)0);
      return 0;}
    if (!initialized) { // first time. Must initialize table
      // make table of ln(n!)
      double sum = fac_table[0] = 0.;
      for (int i=1; i<FAK_LEN; i++) {
        sum += log((double)i);
        fac_table[i] = sum;}
      initialized = 1;}
    return fac_table[n];}
    
  // not found in table. use Stirling approximation
  double  n1, r;
  n1 = n;  r  = 1. / n1;
  return (n1 + 0.5)*log(n1) - n1 + C0 + r*(C1 + r*r*C3);}


/***********************************************************************
                      constants
***********************************************************************/
const double SHAT1 = 2.943035529371538573;    // 8/e
const double SHAT2 = 0.8989161620588987408;   // 3-sqrt(12/e)

  
/***********************************************************************
                      Poisson distribution
***********************************************************************/
int32 StochasticLib1::Poisson (double L) {
/*
   This function generates a random variate with the poisson distribution.

   Uses inversion by chop-down method for L < 17, and ratio-of-uniforms
   method for L >= 17.

   For L < 1.E-6 numerical inaccuracy is avoided by direct calculation.
*/
 
  //------------------------------------------------------------------
  //                 choose method
  //------------------------------------------------------------------
  if (L < 17) {
    if (L < 1.E-6) {
      if (L == 0) return 0;
      if (L < 0) FatalError("Parameter negative in poisson function");
    
      //--------------------------------------------------------------
      // calculate probabilities
      //--------------------------------------------------------------
      // For extremely small L we calculate the probabilities of x = 1
      // and x = 2 (ignoring higher x). The reason for using this 
      // method is to prevent numerical inaccuracies in other methods.
      //--------------------------------------------------------------
      return PoissonLow(L);}
    
    else {
    
      //--------------------------------------------------------------
      // inversion method
      //--------------------------------------------------------------
      // The computation time for this method grows with L.
      // Gives overflow for L > 80
      //--------------------------------------------------------------
      return PoissonInver(L);}}
      
  else {
    if (L > 2.E9) FatalError("Parameter too big in poisson function");

    //----------------------------------------------------------------
    // ratio-of-uniforms method
    //----------------------------------------------------------------
    // The computation time for this method does not depend on L.
    // Use where other methods would be slower.
    //----------------------------------------------------------------
    return PoissonRatioUniforms(L);}}

    
/***********************************************************************
                      Binomial distribution
***********************************************************************/
int32 StochasticLib1::Binomial (int32 n, double p) {
/*
   This function generates a random variate with the binomial distribution.

   Uses inversion by chop-down method for n*p < 35, and ratio-of-uniforms
   method for n*p >= 35.

   For n*p < 1.E-6 numerical inaccuracy is avoided by poisson approximation.
*/
  int inv = 0;            // invert
  int32 x;                // result
  double np = n * p;

  if (p > 0.5) {  // faster calculation by inversion
    p = 1. - p;  inv = 1;}

  if (n <= 0 || p <= 0) {
    if (n == 0 || p == 0) return inv * n;  // only one possible result
    FatalError("Parameter out of range in binomial function");} // error exit

  //------------------------------------------------------------------
  //                 choose method
  //------------------------------------------------------------------
  if (np < 35.) {
    if (np < 1.E-6) {
      // Poisson approximation for extremely low np
      x = PoissonLow(np);}

    else {
      // inversion method, using chop-down search from 0
      x = BinomialInver(n, p);}}
  
  else {
    // ratio of uniforms method
    x = BinomialRatioOfUniforms(n, p);}

  if (inv) {
    x = n - x;}      // undo inversion
  return x;}


/***********************************************************************
                      Hypergeometric distribution
***********************************************************************/
int32 StochasticLib1::Hypergeometric (int32 n, int32 m, int32 N) {
/*
   This function generates a random variate with the hypergeometric
   distribution. This is the distribution you get when drawing balls without 
   replacement from an urn with two colors. n is the number of balls you take,
   m is the number of red balls in the urn, N is the total number of balls in 
   the urn, and the return value is the number of red balls you get.

   This function uses inversion by chop-down search from the mode when
   parameters are small, and the ratio-of-uniforms method when the former
   method would be too slow or would give overflow.
*/   

  int32 fak, addd;           // used for undoing transformations
  int32 x;                   // result

  // check if parameters are valid
  if (n > N || m > N || n < 0 || m < 0) {
    FatalError("Parameter out of range in hypergeometric function");}

  // symmetry transformations
  fak = 1;  addd = 0;
  if (m > N/2) {
    // invert m
    m = N - m;
    fak = -1;  addd = n;}
    
  if (n > N/2) {
    // invert n
    n = N - n;
    addd += fak * m;  fak = - fak;}
    
  if (n > m) {
    // swap n and m
    x = n;  n = m;  m = x;}
    
  // cases with only one possible result end here
  if (n == 0)  return addd;

  //------------------------------------------------------------------
  //                 choose method
  //------------------------------------------------------------------
  if (N > 680 || n > 70) {
    // use ratio-of-uniforms method
    x = HypRatioOfUnifoms (n, m, N);}

  else {
    // inversion method, using chop-down search from mode
    x = HypInversionMod (n, m, N);}

  // undo symmetry transformations  
  return x * fak + addd;}

  
/***********************************************************************
                      Normal distribution
***********************************************************************/
  
double StochasticLib1::Normal(double m, double s) {
  // normal distribution with mean m and standard deviation s
  double normal_x1;          // first random coordinate (normal_x2 is member of class)
  double w;                  // radius
  if (normal_x2_valid) {     // we have a valid result from last call
    normal_x2_valid = 0;
    return normal_x2 * s + m;}
    
  // make two normally distributed variates by Box-Muller transformation
  do {
    normal_x1 = 2. * Random() - 1.;
    normal_x2 = 2. * Random() - 1.;
    w = normal_x1*normal_x1 + normal_x2*normal_x2;}
  while (w >= 1. || w < 1E-30);
  w = sqrt(log(w)*(-2./w));
  normal_x1 *= w;  normal_x2 *= w;     // normal_x1 and normal_x2 are independent normally distributed variates
  normal_x2_valid = 1;                 // save normal_x2 for next call
  return normal_x1 * s + m;}

  
/***********************************************************************
                      Bernoulli distribution
***********************************************************************/
int StochasticLib1::Bernoulli(double p) {
  // Bernoulli distribution with parameter p. This function returns 
  // 0 or 1 with probability (1-p) and p, respectively.
  if (p < 0 || p > 1) FatalError("Parameter out of range in Bernoulli function");
  return Random() < p;}


/***********************************************************************
                      Multinomial distribution
***********************************************************************/
void StochasticLib1::Multinomial (int32 * destination, double * source, int32 n, int colors) {
/*
   This function generates a vector of random variates, each with the
   binomial distribution.

   The multinomial distribution is the distribution you get when drawing
   balls from an urn with more than two colors, with replacement.

   Parameters:
   destination:    An output array to receive the number of balls of each 
                   color. Must have space for at least 'colors' elements.
   source:         An input array containing the probability or fraction 
                   of each color in the urn. Must have 'colors' elements.
                   All elements must be non-negative. The sum doesn't have
                   to be 1, but the sum must be positive.
   n:              The number of balls drawn from the urn.                   
   colors:         The number of possible colors. 
*/
  double s, sum;
  int32 x;
  int i;
  if (n < 0 || colors < 0) FatalError("Parameter negative in multinomial function");
  if (colors == 0) return;
  
  // compute sum of probabilities
  for (i=0, sum=0; i<colors; i++) { 
    s = source[i];
    if (s < 0) FatalError("Parameter negative in multinomial function");
    sum += s;}
  if (sum == 0 && n > 0) FatalError("Zero sum in multinomial function");

  for (i=0; i<colors-1; i++) { 
    // generate output by calling binomial (colors-1) times
    s = source[i];
    if (sum <= s) {
      // this fixes two problems:
      // 1. prevent division by 0 when sum = 0
      // 2. prevent s/sum getting bigger than 1 in case of rounding errors
      x = n;}
    else {    
      x = Binomial(n, s/sum);}
    n -= x; sum -= s;
    destination[i] = x;}
  // get the last one
  destination[i] = n;}


void StochasticLib1::Multinomial (int32 * destination, int32 * source, int32 n, int colors) {
  // same as above, with integer source
  int32 x, p, sum;
  int i;
  if (n < 0 || colors < 0) FatalError("Parameter negative in multinomial function");
  if (colors == 0) return;
  
  // compute sum of probabilities
  for (i=0, sum=0; i<colors; i++) { 
    p = source[i];
    if (p < 0) FatalError("Parameter negative in multinomial function");
    sum += p;}
  if (sum == 0 && n > 0) FatalError("Zero sum in multinomial function");

  for (i=0; i<colors-1; i++) { 
    // generate output by calling binomial (colors-1) times
    if (sum == 0) {
      destination[i] = 0; continue;}
    p = source[i];
    x = Binomial(n, (double)p/sum);
    n -= x; sum -= p;
    destination[i] = x;}
  // get the last one
  destination[i] = n;}


/***********************************************************************
                  Multivariate hypergeometric distribution
***********************************************************************/
void StochasticLib1::MultiHypergeometric (int32 * destination, int32 * source, int32 n, int colors) {
/*
   This function generates a vector of random variates, each with the
   hypergeometric distribution.

   The multivariate hypergeometric distribution is the distribution you 
   get when drawing balls from an urn with more than two colors, without
   replacement.

   Parameters:
   destination:    An output array to receive the number of balls of each 
                   color. Must have space for at least 'colors' elements.
   source:         An input array containing the number of balls of each 
                   color in the urn. Must have 'colors' elements.
                   All elements must be non-negative.
   n:              The number of balls drawn from the urn.
                   Can't exceed the total number of balls in the urn.
   colors:         The number of possible colors. 
*/
  int32 sum, x, y;
  int i;
  if (n < 0 || colors < 0) FatalError("Parameter negative in multihypergeo function");
  if (colors == 0) return;
  
  // compute total number of balls
  for (i=0, sum=0; i<colors; i++) { 
    y = source[i];
    if (y < 0) FatalError("Parameter negative in multihypergeo function");
    sum += y;}
  if (n > sum) FatalError("n > sum in multihypergeo function");

  for (i=0; i<colors-1; i++) { 
    // generate output by calling hypergeometric colors-1 times
    y = source[i];
    x = Hypergeometric(n, y, sum);
    n -= x; sum -= y;
    destination[i] = x;}
  // get the last one
  destination[i] = n;}



/***********************************************************************
                      Shuffle function
***********************************************************************/
void StochasticLib1::Shuffle(int * list, int min, int n) {
/*
   This function makes a list of the n numbers from min to min+n-1
   in random order.

   The parameter 'list' must be an array with at least n elements.
   The array index goes from 0 to n-1.

   If you want to shuffle something else than integers then use the 
   integers in list as an index into a table of the items you want to shuffle.
*/

  int i, j, swap;
  // put numbers from min to min+n-1 into list
  for (i=0, j=min; i<n; i++, j++) list[i] = j;
  // shuffle list
  for (i=0; i<n-1; i++) {
    // item number i has n-i numbers to choose between
    j = IRandom(i,n-1);
    // swap items i and j
    swap = list[j];  list[j] = list[i];  list[i] = swap;}}
  

/***********************************************************************
                      Subfunctions used by poisson
***********************************************************************/
int32 StochasticLib1::PoissonLow(double L) {
/*
   This subfunction generates a random variate with the poisson 
   distribution for extremely low values of L.

   The method is a simple calculation of the probabilities of x = 1
   and x = 2. Higher values are ignored.

   The reason for using this method is to avoid the numerical inaccuracies 
   in other methods.
*/   
  double d, r;
  d = sqrt(L);
  if (Random() >= d) return 0;
  r = Random() * d;
  if (r > L * (1.-L)) return 0;
  if (r > 0.5 * L*L * (1.-L)) return 1;
  return 2;}
    

int32 StochasticLib1::PoissonInver(double L) {
/*
   This subfunction generates a random variate with the poisson 
   distribution using inversion by the chop down method (PIN).

   Execution time grows with L. Gives overflow for L > 80.

   The value of bound must be adjusted to the maximal value of L.
*/   
  const int bound = 130;             // safety bound. Must be > L + 8*sqrt(L).
  static double p_L_last = -1.;      // previous value of L
  static double p_f0;                // value at x=0
  double r;                          // uniform random number
  double f;                          // function value
  int32 x;                           // return value

  if (L != p_L_last) {               // set up
    p_L_last = L;
    p_f0 = exp(-L);}                 // f(0) = probability of x=0

  while (1) {  
    r = Random();  x = 0;  f = p_f0;
    do {                        // recursive calculation: f(x) = f(x-1) * L / x
      r -= f;
      if (r <= 0) return x;
      x++;
      f *= L;
      r *= x;}                       // instead of f /= x
    while (x <= bound);}}
  
  
int32 StochasticLib1::PoissonRatioUniforms(double L) {
/*
   This subfunction generates a random variate with the poisson 
   distribution using the ratio-of-uniforms rejection method (PRUAt).

   Execution time does not depend on L, except that it matters whether L
   is within the range where ln(n!) is tabulated.

   Reference: E. Stadlober: "The ratio of uniforms approach for generating
   discrete random variates". Journal of Computational and Applied Mathematics,
   vol. 31, no. 1, 1990, pp. 181-189.
*/
  static double p_L_last = -1.0;            // previous L
  static double p_a;                       // hat center
  static double p_h;                       // hat width
  static double p_g;                       // ln(L)
  static double p_q;                       // value at mode
  static int32 p_bound;                    // upper bound
  int32 mode;                              // mode
  double u;                                // uniform random
  double lf;                               // ln(f(x))
  double x;                                // real sample
  int32 k;                                 // integer sample

  if (p_L_last != L) {
    p_L_last = L;                           // Set-up
    p_a = L + 0.5;                          // hat center
    mode = (int32)L;                        // mode
    p_g  = log(L);
    p_q = mode * p_g - LnFac(mode);         // value at mode
    p_h = sqrt(SHAT1 * (L+0.5)) + SHAT2;    // hat width
    p_bound = (int32)(p_a + 6.0 * p_h);}    // safety-bound

  while(1) {
    u = Random();
    if (u == 0) continue;                   // avoid division by 0
    x = p_a + p_h * (Random() - 0.5) / u;
    if (x < 0 || x >= p_bound) continue;    // reject if outside valid range
    k = (int32)(x);
    lf = k * p_g - LnFac(k) - p_q;
    if (lf >= u * (4.0 - u) - 3.0) break;   // quick acceptance
    if (u * (u - lf) > 1.0) continue;       // quick rejection
    if (2.0 * log(u) <= lf) break;}         // final acceptance
  return(k);}
   

/***********************************************************************
                      Subfunctions used by binomial
***********************************************************************/

int32 StochasticLib1::BinomialInver (int32 n, double p) {
/* 
  Subfunction for Binomial distribution. Assumes p < 0.5.

  Uses inversion method by search starting at 0.

  Gives overflow for n*p > 60.
  
  This method is fast when n*p is low. 
*/   
  double f0, f, q; 
  int32 bound;
  double pn, r, rc; 
  int32 x, n1, i;
    
  // f(0) = probability of x=0 is (1-p)^n
  // fast calculation of (1-p)^n
  f0 = 1.;  pn = 1.-p;  n1 = n;
  while (n1) {
    if (n1 & 1) f0 *= pn;
    pn *= pn;  n1 >>= 1;}

  // calculate safety bound
  rc = (n + 1) * p;
  bound = (int32)(rc + 11.0*(sqrt(rc) + 1.0));
  if (bound > n) bound = n; 
  q = p / (1. - p);

  while (1) {
    r = Random();
    // recursive calculation: f(x) = f(x-1) * (n-x+1)/x*p/(1-p)
    f = f0;  x = 0;  i = n;
    do {
      r -= f;
      if (r <= 0) return x;
      x++;
      f *= q * i;
      r *= x;       // it is faster to multiply r by x than dividing f by x
      i--;}
    while (x <= bound);}}


int32 StochasticLib1::BinomialRatioOfUniforms (int32 n, double p) {
/* 
  Subfunction for Binomial distribution. Assumes p < 0.5.

  Uses the Ratio-of-Uniforms rejection method (BRUAt).

  The computation time hardly depends on the parameters, except that it matters
  a lot whether parameters are within the range where the LnFac function is 
  tabulated.
  
  Reference: E. Stadlober: "The ratio of uniforms approach for generating
  discrete random variates". Journal of Computational and Applied Mathematics,
  vol. 31, no. 1, 1990, pp. 181-189.
*/   
  static int32 b_n_last = -1;               // last n
  static double b_p_last = -1.;             // last p
  static int32 b_mode;                      // mode
  static int32 b_bound;                     // upper bound
  static double b_a;                        // hat center
  static double b_h;                        // hat width
  static double b_g;                        // value at mode
  static double b_r1;                       // ln(p/(1-p))
  double u;                                 // uniform random
  double q1;                                // 1-p
  double np;                                // n*p
  double var;                               // variance
  double lf;                                // ln(f(x))
  double x;                                 // real sample
  int32 k;                                  // integer sample

  if(b_n_last != n || b_p_last != p) {      // Set_up
    b_n_last = n;
    b_p_last = p;
    q1 = 1.0 - p;
    np = n * p;
    b_mode = (int32)(np + p);               // mode
    b_a = np + 0.5;                         // hat center
    b_r1 = log(p / q1);
    b_g = LnFac(b_mode) + LnFac(n-b_mode);
    var = np * q1;                          // variance
    b_h = sqrt(SHAT1 * (var+0.5)) + SHAT2;  // hat width
    b_bound = (int32)(b_a + 6.0 * b_h);     // safety-bound
    if (b_bound > n) b_bound = n;}          // safety-bound
      
  while (1) {                               // rejection loop
    u = Random();
    if (u == 0) continue;                   // avoid division by 0
    x = b_a + b_h * (Random() - 0.5) / u;
    if (x < 0. || x > b_bound) continue;    // reject, avoid overflow
    k = (int32)x;                           // truncate
    lf = (k-b_mode)*b_r1+b_g-LnFac(k)-LnFac(n-k);// ln(f(k))
    if (u * (4.0 - u) - 3.0 <= lf) break;   // lower squeeze accept
    if (u * (u - lf) > 1.0) continue;       // upper squeeze reject
    if (2.0 * log(u) <= lf) break;}         // final acceptance
  return k;}

  
/***********************************************************************
                      Subfunctions used by hypergeometric
***********************************************************************/

int32 StochasticLib1::HypInversionMod (int32 n, int32 m, int32 N) {
/* 
  Subfunction for Hypergeometric distribution. Assumes 0 <= n <= m <= N/2.
  Overflow protection is needed when N > 680 or n > 75.

  Hypergeometric distribution by inversion method, using down-up 
  search starting at the mode using the chop-down technique.

  This method is faster than the rejection method when the variance is low.
*/   

  static int32  h_n_last = -1, h_m_last = -1, h_N_last = -1;
  static int32  h_mode, h_mp, h_bound;
  static double h_fm;
  int32         L, I, K;
  double        modef, Mp, np, p, c, d, U, divisor;

  Mp = (double)(m + 1);
  np = (double)(n + 1);
  L = N - m - n;
  
  if (N != h_N_last || m != h_m_last || n != h_n_last) {
    // set-up when parameters have changed
    h_N_last = N;  h_m_last = m;  h_n_last = n;

    p  = Mp / (N + 2.);
    modef = np * p;                                   // mode, real
    h_mode = (int32)modef;                            // mode, integer
    if (h_mode == modef && p == 0.5) {   
      h_mp = h_mode--;}
    else {
      h_mp = h_mode + 1;}

    // mode probability, using log factorial function
    // (may read directly from fac_table if N < FAK_LEN)
    h_fm = exp(LnFac(N-m) - LnFac(L+h_mode) - LnFac(n-h_mode)
             + LnFac(m)   - LnFac(m-h_mode)    - LnFac(h_mode)
             - LnFac(N)   + LnFac(N-n)         + LnFac(n)        );

    // safety bound - guarantees at least 17 significant decimal digits
    // bound = min(n, (int32)(modef + k*c'))
    h_bound = (int32)(modef + 11. * sqrt(modef * (1.-p) * (1.-n/(double)N)+1.));
    if (h_bound > n) h_bound = n;}

  // loop until accepted
  while(1) {
    U = Random();                  // uniform random number to be converted
    
    if ((U -= h_fm) <= 0.) return(h_mode);
    c = d = h_fm;

    // alternating down- and upward search from the mode
    for (I = 1; I <= h_mode; I++) {
      K  = h_mp - I;                                  // downward search
      divisor = (np - K)*(Mp - K);
      // Instead of dividing c with divisor, we multiply U and d because 
      // multiplication is faster. This will give overflow if N > 800
      U *= divisor;  d *= divisor;
      c *= (double)K * (double)(L + K);
      if ((U -= c) <= 0.)  return(K - 1);

      K  = h_mode + I;                                // upward search
      divisor = (double)K * (double)(L + K);
      U *= divisor;  c *= divisor; // re-scale parameters to avoid time-consuming division
      d *= (np - K) * (Mp - K);
      if ((U -= d) <= 0.)  return(K);
      // Values of n > 75 or N > 680 may give overflow if you leave out this..
      // overflow protection
      // if (U > 1.E100) {U *= 1.E-100; c *= 1.E-100; d *= 1.E-100;}
      }

    // upward search from K = 2*mode + 1 to K = bound
    for (K = h_mp + h_mode; K <= h_bound; K++) {
      divisor = (double)K * (double)(L + K);
      U *= divisor;
      d *= (np - K) * (Mp - K);
      if ((U -= d) <= 0.)  return(K);
      // more overflow protection
      // if (U > 1.E100) {U *= 1.E-100; d *= 1.E-100;}
      }}}


int32 StochasticLib1::HypRatioOfUnifoms (int32 n, int32 m, int32 N) {
/* 
  Subfunction for Hypergeometric distribution using the ratio-of-uniforms
  rejection method.

  This code is valid for 0 < n <= m <= N/2.

  The computation time hardly depends on the parameters, except that it matters
  a lot whether parameters are within the range where the LnFac function is 
  tabulated.
  
  Reference: E. Stadlober: "The ratio of uniforms approach for generating
  discrete random variates". Journal of Computational and Applied Mathematics,
  vol. 31, no. 1, 1990, pp. 181-189.
*/  
  static int32 h_N_last = -1, h_m_last = -1, h_n_last = -1;// previous parameters
  static int32 h_bound;                                    // upper bound
  static double h_a;                                       // hat center
  static double h_h;                                       // hat width
  static double h_g;                                       // value at mode
  int32 L;                                                 // N-m-n
  int32 mode;                                              // mode
  int32 k;                                                 // integer sample
  double x;                                                // real sample
  double rNN;                                              // 1/(N*(N+2))
  double my;                                               // mean
  double var;                                              // variance
  double u;                                                // uniform random
  double lf;                                               // ln(f(x))

  L = N - m - n;
  if (h_N_last != N || h_m_last != m || h_n_last != n) {
    h_N_last = N;  h_m_last = m;  h_n_last = n;            // Set-up
    rNN = 1. / ((double)N*(N+2));                          // make two divisions in one
    my = (double)n * m * rNN * (N+2);                      // mean = n*m/N
    mode = (int32)(double(n+1) * double(m+1) * rNN * N);   // mode = floor((n+1)*(m+1)/(N+2))
    var = (double)n * m * (N-m) * (N-n) / ((double)N*N*(N-1)); // variance
    h_h = sqrt(SHAT1 * (var+0.5)) + SHAT2;                 // hat width
    h_a = my + 0.5;                                        // hat center
    h_g = fc_lnpk(mode, L, m, n);                          // maximum
    h_bound = (int32)(h_a + 4.0 * h_h);                    // safety-bound
    if (h_bound > n) h_bound = n;}
    
  while(1) {
    u = Random();                                          // uniform random number
    if (u == 0) continue;                                  // avoid division by 0
    x = h_a + h_h * (Random()-0.5) / u;                    // generate hat distribution
    if (x < 0. || x > 2E9) continue;                       // reject, avoid overflow
    k = (int32)x;
    if (k > h_bound) continue;                             // reject if outside range
    lf = h_g - fc_lnpk(k,L,m,n);                           // ln(f(k))
    if (u * (4.0 - u) - 3.0 <= lf) break;                  // lower squeeze accept
    if (u * (u-lf) > 1.0) continue;                        // upper squeeze reject
    if (2.0 * log(u) <= lf) break;}                        // final acceptance

  return k;}

  

/***********************************************************************
                  Subfunctions used by several functions
***********************************************************************/

double StochasticLib1::fc_lnpk(int32 k, int32 L, int32 m, int32 n) {
  // subfunction used by hypergeometric and extended hypergeometric distribution
  return(LnFac(k) + LnFac(m - k) + LnFac(n - k) + LnFac(L + k));}

  
/***********************************************************************
                      Constructor
***********************************************************************/
StochasticLib1::StochasticLib1 (int seed)
  : RANDOM_GENERATOR(seed) {
  normal_x2_valid = 0;}
