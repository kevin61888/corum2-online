/***************************** STOC2.CPP *********************** 2001-11-04 AF *
*
* Non-uniform random number generator functions.
*
* This file contains source code for the class StochasticLib2 defined in stocc.h.
*
* Documentation:
* ==============
* The file stocc.h contains class definitions.
* The file stocc.htm contains further instructions.
* The file distrib.pdf contains definitions of the statistic distributions.
* The file sampmet.pdf contains theoretical descriptions of the methods used
* for sampling from these distributions.
*
* © 2002 Agner Fog. GNU General Public License www.gnu.org/copyleft/gpl.html
*******************************************************************************/

#include <math.h>      // math functions
#include "stocc.h"     // class definition

  
/***********************************************************************
                      Poisson distribution
***********************************************************************/
int32 StochasticLib2::Poisson (double L) {
/*
   This function generates a random variate with the poisson distribution.

   Uses down/up search from the mode by chop-down technique for L < 20,
   and patchwork rejection method for L >= 20.

   For L < 1.E-6 numerical inaccuracy is avoided by direct calculation.
*/
  //------------------------------------------------------------------
  //                 choose method
  //------------------------------------------------------------------
  if (L < 20) {
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
      // down/up search from mode
      //--------------------------------------------------------------
      // The computation time for this method grows with sqrt(L).
      // Gives overflow for L > 60
      //--------------------------------------------------------------
      return PoissonModeSearch(L);}}
      
  else {
    if (L > 2.E9) FatalError("Parameter too big in Poisson function");

    //----------------------------------------------------------------
    // patchword rejection method
    //----------------------------------------------------------------
    // The computation time for this method does not depend on L.
    // Use where other methods would be slower.
    //----------------------------------------------------------------
    return PoissonPatchwork(L);}}


/***********************************************************************
                      Binomial distributuion
***********************************************************************/
int32 StochasticLib2::Binomial (int32 n, double p) {
/*
   This function generates a random variate with the binomial distribution.

   Uses down/up search from the mode by chop-down technique for n*p < 60,
   and patchwork rejection method for n*p >= 55.

   For n*p < 1.E-6 numerical inaccuracy is avoided by poisson approximation.
*/
  int inv = 0;            // invert
  int32 x;                // result
  double np = n * p;

  if (p > 0.5) {  // faster calculation by inversion
    p = 1. - p;  inv = 1;}

  if (n <= 0 || p <= 0) {
    if (n == 0 || p == 0) return inv * n;  // only one possible result
    FatalError("Parameter negative in binomial function");}  // error exit


  //------------------------------------------------------------------
  //                 choose method
  //------------------------------------------------------------------
  if (np < 55.) {
    if (np < 1.E-6) {
      // Poisson approximation for extremely low np
      x = PoissonLow(np);}

    else {
      // inversion method, using chop-down search from 0
      x = BinomialModeSearch(n, p);}}
  
  else {
    // ratio of uniforms method
    x = BinomialPatchwork(n, p);}

  if (inv) {
    x = n - x;}      // undo inversion
  return x;}

  
/***********************************************************************
                    Hypergeometric distribution
***********************************************************************/
int32 StochasticLib2::Hypergeometric (int32 n, int32 m, int32 N) {
/*
   This function generates a random variate with the hypergeometric
   distribution. This is the distribution you get when drawing balls 
   without replacement from an urn with two colors.

   Uses inversion by chop-down search from the mode when the mean < 20
   and the patchwork-rejection method when the mean > 20.
*/   

  int32 x;                   // result
  int32 fak, addd;           // used for undoing transformations

  // check if parameters are valid
  if (n > N || m > N || n < 0 || m < 0) {
    FatalError("Parameter out of range in hypergeometric function");}

  // transformations
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
  if (double(n) * m >= 20. * N) {
    // use ratio-of-uniforms method
    x = HypPatchwork (n, m, N);}

  else {
    // inversion method, using chop-down search from mode
    x = HypInversionMod (n, m, N);}

  // undo transformations  
  return x * fak + addd;}



/***********************************************************************
                  Subfunctions used by binomial
***********************************************************************/

int32 StochasticLib2::BinomialModeSearch(int32 n, double p) {
/* 
  Subfunction for Binomial distribution. Assumes p < 0.5.

  Uses inversion method by down-up search starting at the mode (BMDU).

  Gives overflow for n*p > 60.
  
  This method is fast when n*p is low. 
*/   
  static int32   B_n_last = -1;
  static double  B_p_last = -1.;
  static int32   B_bound, B_mode;
  static double  B_pq, B_fm;
  int32          K, x;
  double         U, c, d, rc, divisor;

  if (n != B_n_last || p != B_p_last) {
    B_n_last = n;
    B_p_last = p;
    rc = (n + 1) * p;

    // safety bound guarantees at least 17 significant decimal digits
    B_bound = (int32)(rc + 11.0*(sqrt(rc) + 1.0));
    if (B_bound > n)  B_bound = n;
    B_mode = (int32) rc;
    if (B_mode == rc && p == 0.5) B_mode--;    // mode
    B_pq = p / (1.0 - p);
    B_fm = exp(LnFac(n)-LnFac(B_mode)-LnFac(n-B_mode) + B_mode*log(p) + (n-B_mode)*log(1.-p));}
    
  while (1) {
    U = Random();
    if ((U -= B_fm) <= 0.0) return(B_mode);
    c = d = B_fm;

    // down- and upward search from the mode
    for (K = 1; K <= B_mode; K++) {
      x = B_mode - K;                        // downward search from mode
      divisor = (n-x) * B_pq;
      c *= x + 1;
      U *= divisor;
      d *= divisor;
      if ((U -= c) <= 0.0) return x;

      x = B_mode + K;                        // upward search from mode
      divisor = x;
      d *= (n-x+1) * B_pq;
      U *= divisor;
      c *= divisor;      
      if ((U -= d) <= 0.0) return x;}

    // upward search from 2*mode + 1 to bound
    for (x = B_mode + B_mode + 1; x <= B_bound; x++) {
      d *= (n-x+1) * B_pq;
      U *= x;
      if ((U -= d) <= 0.0) return x;}}}


int32 StochasticLib2::BinomialPatchwork(int32 n, double p) {
/*
  Subfunction for Binomial distribution using the patchwork rejection
  method (BPRS).
*/  

  static int32  B_n_last = -1;
  static double B_p_last = -1.;
  static int32  B_k1, B_k2, B_k4, B_k5;
  static double B_dl, B_dr, B_r1, B_r2, B_r4, B_r5, 
                B_ll, B_lr, B_l_pq, B_c_pm,
                B_f1, B_f2, B_f4, B_f5, 
                B_p1, B_p2, B_p3, B_p4, B_p5, B_p6;
  int32         mode, Dk, X, Y;
  double        nu, q, U, V, W;

  if (n != B_n_last || p != B_p_last) {               // set-up
    B_n_last = n;
    B_p_last = p;

    nu = (double)(n + 1) * p;  q = 1.0 - p;           // main parameters

    // approximate deviation of reflection points k2, k4 from nu - 1/2
    W  = sqrt(nu * q + 0.25);

    // mode, reflection points k2 and k4, and points k1 and k5, which
    // delimit the centre region of h(x)
    mode = (int32) nu;
    B_k2 = (int32) ceil(nu - 0.5 - W);
    B_k4 = (int32)     (nu - 0.5 + W);
    B_k1 = B_k2 + B_k2 - mode + 1;
    B_k5 = B_k4 + B_k4 - mode;

    // range width of the critical left and right centre region
    B_dl = (double) (B_k2 - B_k1);
    B_dr = (double) (B_k5 - B_k4);

    // recurrence constants r(k) = p(k)/p(k-1) at k = k1, k2, k4+1, k5+1
    nu = nu / q;  p = p / q;
    B_r1 = nu / (double) B_k1      - p;                   // nu = (n+1)p / q
    B_r2 = nu / (double) B_k2      - p;                   //  p =      p / q
    B_r4 = nu / (double)(B_k4 + 1) - p;
    B_r5 = nu / (double)(B_k5 + 1) - p;

    // reciprocal values of the scale parameters of expon. tail envelopes
    B_ll =  log(B_r1);                                    // expon. tail left
    B_lr = -log(B_r5);                                    // expon. tail right

    // binomial constants, necessary for computing function values f(k)
    B_l_pq = log(p);
    B_c_pm = mode * B_l_pq - LnFac(mode) - LnFac(n - mode);

    // function values f(k) = p(k)/p(mode) at k = k2, k4, k1, k5
    B_f2 = BinomialF(B_k2, n, B_l_pq, B_c_pm);
    B_f4 = BinomialF(B_k4, n, B_l_pq, B_c_pm);
    B_f1 = BinomialF(B_k1, n, B_l_pq, B_c_pm);
    B_f5 = BinomialF(B_k5, n, B_l_pq, B_c_pm);

    // area of the two centre and the two exponential tail regions
    // area of the two immediate acceptance regions between k2, k4
    B_p1 = B_f2 * (B_dl + 1.);                             // immed. left
    B_p2 = B_f2 * B_dl         + B_p1;                     // centre left
    B_p3 = B_f4 * (B_dr + 1.)  + B_p2;                     // immed. right
    B_p4 = B_f4 * B_dr         + B_p3;                     // centre right
    B_p5 = B_f1 / B_ll         + B_p4;                     // expon. tail left
    B_p6 = B_f5 / B_lr         + B_p5;                     // expon. tail right
    }

  for (;;) {
    // generate uniform number U -- U(0, p6)
    // case distinction corresponding to U

    if ((U = Random() * B_p6) < B_p2) {        // centre left
      // immediate acceptance region R2 = [k2, mode) *[0, f2),  X = k2, ... mode -1
      if ((V = U - B_p1) < 0.) return(B_k2 + (int32)(U/B_f2));
      // immediate acceptance region R1 = [k1, k2)*[0, f1),  X = k1, ... k2-1
      if ((W = V / B_dl) < B_f1) return(B_k1 + (int32)(V/B_f1));

      // computation of candidate X < k2, and its counterpart Y > k2
      // either squeeze-acceptance of X or acceptance-rejection of Y
      Dk = (int32)(B_dl * Random()) + 1;
      if (W <= B_f2 - Dk * (B_f2 - B_f2/B_r2)) {        // quick accept of
        return(B_k2 - Dk);}                             // X = k2 - Dk
      if ((V = B_f2 + B_f2 - W) < 1.) {                 // quick reject of Y
        Y = B_k2 + Dk;
        if (V <= B_f2 + Dk * (1. - B_f2)/(B_dl + 1.)) { // quick accept of
          return(Y);}                                   // Y = k2 + Dk
        if (V <= BinomialF(Y,n,B_l_pq,B_c_pm)) return(Y);}// final accept of Y
      X = B_k2 - Dk;}
    
    else if (U < B_p4) {                                // centre right
      // immediate acceptance region R3 = [mode, k4+1)*[0, f4), X = mode, ... k4
      if ((V = U - B_p3) < 0.) return(B_k4 - (int32)((U - B_p2)/B_f4));
      // immediate acceptance region R4 = [k4+1, k5+1)*[0, f5)
      if ((W = V / B_dr) < B_f5) return(B_k5 - (int32)(V/B_f5));

      // computation of candidate X > k4, and its counterpart Y < k4
      // either squeeze-acceptance of X or acceptance-rejection of Y
      Dk = (int32)(B_dr * Random()) + 1;
      if (W <= B_f4 - Dk * (B_f4 - B_f4*B_r4)) {        // quick accept of
        return(B_k4 + Dk);}                             // X = k4 + Dk
      if ((V = B_f4 + B_f4 - W) < 1.) {                 // quick reject of Y
        Y = B_k4 - Dk;
        if (V <= B_f4 + Dk * (1. - B_f4)/ B_dr) {       // quick accept of
          return(Y);}                                   // Y = k4 - Dk
	if (V <= BinomialF(Y,n,B_l_pq,B_c_pm)) return(Y);}// final accept of Y
      X = B_k4 + Dk;}
    else {
      W = Random();
      if (U < B_p5) {                                   // expon. tail left
        Dk = (int32)(1. - log(W)/B_ll);
        if ((X = B_k1 - Dk) < 0) continue;              // 0 <= X <= k1 - 1
        W *= (U - B_p4) * B_ll;                         // W -- U(0, h(x))
        if (W <= B_f1 - Dk * (B_f1 - B_f1/B_r1)) return(X);}  // quick accept of X
      else {                                            // expon. tail right
        Dk = (int32)(1. - log(W)/B_lr);
        if ((X = B_k5 + Dk) > n ) continue;             // k5 + 1 <= X <= n
        W *= (U - B_p5) * B_lr;                         // W -- U(0, h(x))
        if (W <= B_f5 - Dk * (B_f5 - B_f5*B_r5)) return(X);}} // quick accept of X

    // acceptance-rejection test of candidate X from the original area
    // test, whether  W <= BinomialF(k),    with  W = U*h(x)  and  U -- U(0, 1)
    // log BinomialF(X) = (X - mode)*log(p/q) - log X!(n - X)! + log mode!(n - mode)!
    if (log(W) <= X*B_l_pq - LnFac(X) - LnFac(n - X) - B_c_pm) return(X);}}

  
double StochasticLib2::BinomialF(int32 k, int32 n, double l_pq, double c_pm) {
  // used by BinomialPatchwork
  return exp(k*l_pq - LnFac(k) - LnFac(n - k) - c_pm);}

  
/***********************************************************************
                  Subfunctions used by poisson
***********************************************************************/

int32 StochasticLib2::PoissonModeSearch(double L) {
/*
   This subfunction generates a random variate with the poisson 
   distribution by down/up search from the mode, using the chop-down 
   technique (PMDU).

   Execution time grows with sqrt(L). Gives overflow for L > 60.
*/
  static double P_L_last = -1;
  static double P_f0;
  static int32  P_bound;
  double        r, c, d; 
  int32         x, i, mode;

  mode = (int32)L;
  
  if (L != P_L_last) {  // set up
    P_L_last = L;
    P_bound = (int32)floor(L+0.5 + 7.0 * (sqrt(L+L+1.) + 1.5));// safety-bound
    P_f0 = exp(mode * log(L) - L - LnFac(mode));}        // probability of x=mode

  while (1) {
    r = Random();  
    if ((r -= P_f0) <= 0) return mode;
    c = d = P_f0;
    
    // alternating down/up search from the mode
    for (i=1; i<=mode; i++) {
      // down
      x = mode - i;
      c *= x + 1;
      r *= L; d *= L;
      if ((r -= c) <= 0) return x;
      // up
      x = mode + i;
      d *= L;
      r *= x; c *= x;
      if ((r -= d) <= 0) return x;}
      
    // continue upward search from 2*mode+1 to bound
    for (x = mode + mode + 1; x <= P_bound; x++) {
      d *= L;
      r *= x;
      if ((r -= d) <= 0) return x;}}}
  

int32 StochasticLib2::PoissonPatchwork(double L) {
/*
   This subfunction generates a random variate with the poisson 
   distribution using the Patchwork Rejection method (PPRS):
   The area below the histogram function f(x) is rearranged in
   its body by two point reflections. Within a large center
   interval variates are sampled efficiently by rejection from
   uniform hats. Rectangular immediate acceptance regions speed
   up the generation. The remaining tails are covered by
   exponential functions.
   
   For detailed explanation, see:
   Stadlober, E & Zechner, H: "The Patchwork Rejection Technique for 
   Sampling from Unimodal Distributions". ACM Transactions on Modeling
   and Computer Simulation, vol. 9, no. 1, 1999, p. 59-83.

   This method is valid for L >= 10.

   The computation time hardly depends on L, except that it matters
   a lot whether L is within the range where the LnFac function is 
   tabulated.   
*/

  static double P_L_last = -1.;
  static int32  P_k1, P_k2, P_k4, P_k5;
  static double P_dl, P_dr, P_r1, P_r2, P_r4, P_r5, 
                P_ll, P_lr, P_l_my, P_c_pm,
                P_f1, P_f2, P_f4, P_f5, 
                P_p1, P_p2, P_p3, P_p4, P_p5, P_p6;
  int32         mode, Dk, X, Y;
  double        Ds, U, V, W;
      
  if (L != P_L_last) { // set-up
    P_L_last = L;

    // approximate deviation of reflection points k2, k4 from L - 1/2
    Ds = sqrt(L + 0.25);

    // mode, reflection points k2 and k4, and points k1 and k5, which
    // delimit the centre region of h(x)
    mode = (int32) L;
    P_k2 = (int32) ceil(L - 0.5 - Ds);
    P_k4 = (int32)     (L - 0.5 + Ds);
    P_k1 = P_k2 + P_k2 - mode + 1;
    P_k5 = P_k4 + P_k4 - mode;

    // range width of the critical left and right centre region
    P_dl = (double) (P_k2 - P_k1);
    P_dr = (double) (P_k5 - P_k4);

    // recurrence constants r(k) = p(k)/p(k-1) at k = k1, k2, k4+1, k5+1
    P_r1 = L / (double) P_k1;
    P_r2 = L / (double) P_k2;
    P_r4 = L / (double)(P_k4 + 1);
    P_r5 = L / (double)(P_k5 + 1);

    // reciprocal values of the scale parameters of expon. tail envelopes
    P_ll =  log(P_r1);                                     // expon. tail left
    P_lr = -log(P_r5);                                     // expon. tail right

    // Poisson constants, necessary for computing function values f(k)
    P_l_my = log(L);
    P_c_pm = mode * P_l_my - LnFac(mode);

    // function values f(k) = p(k)/p(mode) at k = k2, k4, k1, k5
    P_f2 = PoissonF(P_k2, P_l_my, P_c_pm);
    P_f4 = PoissonF(P_k4, P_l_my, P_c_pm);
    P_f1 = PoissonF(P_k1, P_l_my, P_c_pm);
    P_f5 = PoissonF(P_k5, P_l_my, P_c_pm);

    // area of the two centre and the two exponential tail regions
    // area of the two immediate acceptance regions between k2, k4
    P_p1 = P_f2 * (P_dl + 1.);                             // immed. left
    P_p2 = P_f2 * P_dl         + P_p1;                     // centre left
    P_p3 = P_f4 * (P_dr + 1.) + P_p2;                      // immed. right
    P_p4 = P_f4 * P_dr         + P_p3;                     // centre right
    P_p5 = P_f1 / P_ll         + P_p4;                     // expon. tail left
    P_p6 = P_f5 / P_lr         + P_p5;}                    // expon. tail right

  for (;;) {
    // generate uniform number U -- U(0, p6)
    // case distinction corresponding to U
    if ((U = Random() * P_p6) < P_p2) {                    // centre left

      // immediate acceptance region R2 = [k2, mode) *[0, f2),  X = k2, ... mode -1
      if ((V = U - P_p1) < 0.0)  return(P_k2 + (int32)(U/P_f2));
      // immediate acceptance region R1 = [k1, k2)*[0, f1),  X = k1, ... k2-1
      if ((W = V / P_dl) < P_f1 )  return(P_k1 + (int32)(V/P_f1));

      // computation of candidate X < k2, and its counterpart Y > k2
      // either squeeze-acceptance of X or acceptance-rejection of Y
      Dk = (int32)(P_dl * Random()) + 1;
      if (W <= P_f2 - Dk * (P_f2 - P_f2/P_r2)) {           // quick accept of
        return(P_k2 - Dk);}                                // X = k2 - Dk
            
      if ((V = P_f2 + P_f2 - W) < 1.0) {                   // quick reject of Y
        Y = P_k2 + Dk;
        if (V <= P_f2 + Dk * (1.0 - P_f2)/(P_dl + 1.0)) {  // quick accept of
          return(Y);}                                      // Y = k2 + Dk
        if (V <= PoissonF(Y, P_l_my, P_c_pm)) return(Y);}  // final accept of Y
      X = P_k2 - Dk;}
          
    else if (U < P_p4) {                                   // centre right
      //  immediate acceptance region R3 = [mode, k4+1)*[0, f4), X = mode, ... k4
      if ((V = U - P_p3) < 0.)  return(P_k4 - (int32)((U - P_p2)/P_f4));
      // immediate acceptance region R4 = [k4+1, k5+1)*[0, f5)
      if ((W = V / P_dr) < P_f5)  return(P_k5 - (int32)(V/P_f5));

      // computation of candidate X > k4, and its counterpart Y < k4
      // either squeeze-acceptance of X or acceptance-rejection of Y
      Dk = (int32)(P_dr * Random()) + 1L;
      if (W <= P_f4 - Dk * (P_f4 - P_f4*P_r4)) {           // quick accept of
        return(P_k4 + Dk);}                                // X = k4 + Dk
      if ((V = P_f4 + P_f4 - W) < 1.0) {                   // quick reject of Y
        Y = P_k4 - Dk;
        if (V <= P_f4 + Dk * (1.0 - P_f4)/ P_dr) {         // quick accept of
          return(Y);}                                      // Y = k4 - Dk
        if (V <= PoissonF(Y, P_l_my, P_c_pm))  return(Y);} // final accept of Y
      X = P_k4 + Dk;}
    else {
      W = Random();
      if (U < P_p5) {                                      // expon. tail left
        Dk = (int32)(1.0 - log(W)/P_ll);
        if ((X = P_k1 - Dk) < 0L)  continue;               // 0 <= X <= k1 - 1
        W *= (U - P_p4) * P_ll;                            // W -- U(0, h(x))
        if (W <= P_f1-Dk * (P_f1-P_f1/P_r1)) return(X);}   // quick accept of X
      else {                                               // expon. tail right
        Dk = (int32)(1.0 - log(W)/P_lr);
        X  = P_k5 + Dk;                                    // X >= k5 + 1
        W *= (U - P_p5) * P_lr;                            // W -- U(0, h(x))
        if (W <= P_f5-Dk * (P_f5-P_f5*P_r5)) return(X);}}  // quick accept of X

    // acceptance-rejection test of candidate X from the original area
    // test, whether  W <= f(k),    with  W = U*h(x)  and  U -- U(0, 1)
    // log f(X) = (X - mode)*log(L) - log X! + log mode!
    if (log(W) <= X * P_l_my - LnFac(X) - P_c_pm) return(X);}}
    
    
double StochasticLib2::PoissonF(int32 k, double l_nu, double c_pm) {
  // used by PoissonPatchwork
  return  exp(k * l_nu - LnFac(k) - c_pm);}


/***********************************************************************
                  Subfunctions used by hypergeometric
***********************************************************************/
  
int32 StochasticLib2::HypPatchwork (int32 n, int32 m, int32 N) {
/* 
  Subfunction for Hypergeometric distribution.

  This method is valid only for mode >= 10 and 0 <= n <= m <= N/2.

  This method is fast when called repeatedly with the same parameters, but
  slow when the parameters change due to a high setup time. The computation
  time hardly depends on the parameters, except that it matters a lot whether
  parameters are within the range where the LnFac function is tabulated.
  
  Uses the Patchwork Rejection method of Heinz Zechner (HPRS).
  The area below the histogram function f(x) in its body is rearranged by 
  two point reflections. Within a large center interval variates are sampled 
  efficiently by rejection from uniform hats. Rectangular immediate acceptance
  regions speed up the generation. The remaining tails are covered by 
  exponential functions.

  For detailed explanation, see:
  Stadlober, E & Zechner, H: "The Patchwork Rejection Technique for 
  Sampling from Unimodal Distributions". ACM Transactions on Modeling
  and Computer Simulation, vol. 9, no. 1, 1999, p. 59-83.
  
*/

  static int32  H_N_last = -1, H_m_last = -1, H_n_last = -1, 
                H_L, H_k1, H_k2, H_k4, H_k5;
  static double H_dl, H_dr, 
                H_r1, H_r2, H_r4, H_r5, 
                H_ll, H_lr, H_c_pm, 
                H_f1, H_f2, H_f4, H_f5, 
                H_p1, H_p2, H_p3, H_p4, H_p5, H_p6;
  int32         mode, Dk, X, V;
  double        Mp, np, p, modef, U, Y, W;                 // (X, Y) <-> (V, W)
  
  if (N != H_N_last || m != H_m_last || n != H_n_last) { 
    // set-up when parameters have changed
    H_N_last = N; H_m_last = m; H_n_last = n;

    Mp = (double)(m + 1);
    np = (double)(n + 1);  
    H_L = N - m - n;

    p  = Mp / (N + 2.);  
    modef = np * p;

    // approximate deviation of reflection points k2, k4 from modef - 1/2
    U  = sqrt(modef * (1. - p) * (1. - (n + 2.)/(N + 3.)) + 0.25);

    // mode, reflection points k2 and k4, and points k1 and k5, which 
    // delimit the centre region of h(x)
    // k2 = ceil (modef - 1/2 - U),    k1 = 2*k2 - (mode - 1 + delta_ml)
    // k4 = floor(modef - 1/2 + U),    k5 = 2*k4 - (mode + 1 - delta_mr)
    mode  = (int32)modef;
    H_k2 = (int32)ceil(modef - 0.5 - U);  
    if (H_k2 >= mode) H_k2 = mode - 1;
    H_k4 = (int32)(modef - 0.5 + U);
    H_k1 = H_k2 + H_k2 - mode + 1;                         // delta_ml = 0
    H_k5 = H_k4 + H_k4 - mode;                             // delta_mr = 1

    // range width of the critical left and right centre region
    H_dl = (double) (H_k2 - H_k1);
    H_dr = (double) (H_k5 - H_k4);

    // recurrence constants r(k) = p(k)/p(k-1) at k = k1, k2, k4+1, k5+1
    H_r1 = (np/(double) H_k1    - 1.) * (Mp - H_k1)/(double)(H_L + H_k1);
    H_r2 = (np/(double) H_k2    - 1.) * (Mp - H_k2)/(double)(H_L + H_k2);
    H_r4 = (np/(double)(H_k4+1) - 1.) * (m  - H_k4)/(double)(H_L + H_k4 + 1);
    H_r5 = (np/(double)(H_k5+1) - 1.) * (m  - H_k5)/(double)(H_L + H_k5 + 1);

    // reciprocal values of the scale parameters of expon. tail envelopes
    H_ll =  log(H_r1);                                     // expon. tail left
    H_lr = -log(H_r5);                                     // expon. tail right

    // hypergeom. constant, necessary for computing function values f(k)
    H_c_pm = fc_lnpk(mode, H_L, m, n);

    // function values f(k) = p(k)/p(mode)  at  k = k2, k4, k1, k5
    H_f2 = exp(H_c_pm - fc_lnpk(H_k2, H_L, m, n));
    H_f4 = exp(H_c_pm - fc_lnpk(H_k4, H_L, m, n));
    H_f1 = exp(H_c_pm - fc_lnpk(H_k1, H_L, m, n));
    H_f5 = exp(H_c_pm - fc_lnpk(H_k5, H_L, m, n));

    // area of the two centre and the two exponential tail regions
    // area of the two immediate acceptance regions between k2, k4
    H_p1 = H_f2 * (H_dl+1.);                               // immed. left
    H_p2 = H_f2 * H_dl      + H_p1;                        // centre left
    H_p3 = H_f4 * (H_dr+1.) + H_p2;                        // immed. right
    H_p4 = H_f4 * H_dr      + H_p3;                        // centre right
    H_p5 = H_f1 / H_ll      + H_p4;                        // expon. tail left
    H_p6 = H_f5 / H_lr      + H_p5;                        // expon. tail right
    }

    while (1) {
      // generate uniform number U -- U(0, p6)
      // case distinction corresponding to U
      if ((U = Random() * H_p6) < H_p2) {                  // centre left

        // immediate acceptance region R2 = [k2, mode) *[0, f2),  X = k2, ... mode -1
        if ((W = U - H_p1) < 0.)  return(H_k2 + (int32)(U/H_f2));
        // immediate acceptance region R1 = [k1, k2)*[0, f1),  X = k1, ... k2-1
        if ((Y = W / H_dl) < H_f1)  return(H_k1 + (int32)(W/H_f1));

        // computation of candidate X < k2, and its reflected counterpart V > k2
        // either squeeze-acceptance of X or acceptance-rejection of V
        Dk = (int32)(H_dl * Random()) + 1;
        if (Y <= H_f2 - Dk * (H_f2 - H_f2/H_r2)) {         // quick accept of
          return(H_k2 - Dk);}                              // X = k2 - Dk

        if ((W = H_f2 + H_f2 - Y) < 1.) {                  // quick reject of V
          V = H_k2 + Dk;
          if (W <= H_f2 + Dk * (1. - H_f2)/(H_dl + 1.)) {  // quick accept of V
            return(V);}
          
          if (log(W) <= H_c_pm - fc_lnpk(V, H_L, m, n)) {
            return(V);}}                                   // final accept of V

        X = H_k2 - Dk;}                                    // go to final accept/reject
        
      else if (U < H_p4) {                                 // centre right

        // immediate acceptance region R3 = [mode, k4+1)*[0, f4), X = mode, ... k4
        if ((W = U - H_p3) < 0.)  return(H_k4 - (int32)((U - H_p2)/H_f4));
        
        // immediate acceptance region R4 = [k4+1, k5+1)*[0, f5)
        if ((Y = W / H_dr) < H_f5)  return(H_k5 - (int32)(W/H_f5));

        // computation of candidate X > k4, and its reflected counterpart V < k4
        // either squeeze-acceptance of X or acceptance-rejection of V
        Dk = (int32)(H_dr * Random()) + 1;
        if (Y <= H_f4 - Dk * (H_f4 - H_f4*H_r4)) {         // quick accept of
          return(H_k4 + Dk);}                              // X = k4 + Dk

        if ((W = H_f4 + H_f4 - Y) < 1.) {                  // quick reject of V
          V = H_k4 - Dk;
          if (W <= H_f4 + Dk * (1. - H_f4)/H_dr) {         // quick accept of
            return(V);}                                    // V = k4 - Dk

          if (log(W) <= H_c_pm - fc_lnpk(V, H_L, m, n)) {
            return(V);}}                                   // final accept of V

        X = H_k4 + Dk;}                                    // go to final accept/reject
      
      else {
        Y = Random();
        if (U < H_p5) {                                    // expon. tail left
          Dk = (int32)(1. - log(Y)/H_ll);
          if ((X = H_k1 - Dk) < 0)  continue;              // 0 <= X <= k1 - 1
          Y *= (U - H_p4) * H_ll;                          // Y -- U(0, h(x))
          if (Y <= H_f1 - Dk * (H_f1 - H_f1/H_r1)) {
            return(X);}}                                   // quick accept of X

        else {                                             // expon. tail right
          Dk = (int32)(1. - log(Y)/H_lr);
          if ((X = H_k5 + Dk) > n )  continue;             // k5 + 1 <= X <= n
          Y *= (U - H_p5) * H_lr;                          // Y -- U(0, h(x))
          if (Y <= H_f5 - Dk * (H_f5 - H_f5*H_r5)) {
            return(X);}}}                                  // quick accept of X

      // acceptance-rejection test of candidate X from the original area
      // test, whether  Y <= f(X),    with  Y = U*h(x)  and  U -- U(0, 1)
      // log f(X) = log( mode! (m - mode)! (n - mode)! (N - m - n + mode)! )
      //          - log( X! (m - X)! (n - X)! (N - m - n + X)! )
      if (log(Y) <= H_c_pm - fc_lnpk(X, H_L, m, n)) return(X);
      }}
