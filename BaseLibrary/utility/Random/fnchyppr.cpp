/************************** FNCHYPPR.CPP ********************** 2002-10-20 AF *
*
* Calculation of univariate and multivariate Fisher's noncentral hypergeometric
* probability distribution.
*
* This file contains source code for the class CFishersNCHypergeometric 
* and CMultiFishersNCHypergeometric defined in stocc.h.
*
* Documentation:
* ==============
* The file stocc.h contains class definitions.
* The file stocc.htm contains further instructions.
*
*******************************************************************************/

#include <math.h>      // math functions
#include <string.h>    // memcpy function
#include "stocc.h"     // class definition

/***********************************************************************
             Methods for class CFishersNCHypergeometric
***********************************************************************/

CFishersNCHypergeometric::CFishersNCHypergeometric(int32 n_, int32 m_, int32 N_, double odds_) {
  // constructor
  // set parameters
  n = n_; m = m_; N = N_; odds = odds_;
  // check validity of parameters
  if (n<0 || m<0 || N<0 || odds<0. || n>N || m>N) FatalError("Parameter out of range in class CFishersNCHypergeometric");
  // initialize
  logodds = log(odds);  scale = rsum = 0.;
  ParametersChanged = 1;
  // calculate xmin and xmax
  xmin = m + n - N;  if (xmin < 0) xmin = 0;
  xmax = n;  if (xmax > m) xmax = m;}


double CFishersNCHypergeometric::mean(void) {
  // find approximate mean
  double a, b;                         // temporaries in calculation
  double mean;                         // mean

  if (odds == 1.) { // simple hypergeometric
    return double(m)*n/N;}

  // calculate Cornfield mean
  a = (m+n)*odds + (N-m-n); 
  b = a*a - 4.*odds*(odds-1.)*m*n;
  b = b > 0. ? sqrt(b) : 0.;
  mean = (a-b)/(2.*(odds-1.));
  return mean;}

double CFishersNCHypergeometric::moments(double * mean_, double * var_) {
  // calculate exact mean and variance
  // return value = sum of f(x), expected = 1.
  double y, sy=0, sxy=0, sxxy=0, me1;
  int32 x, xm, x1;
  const float accuracy = 1E-10f;       // accuracy of calculation
  xm = (int32)mean();                  // approximation to mean
  for (x=xm; x<=xmax; x++) {
    y = probability(x);
    x1 = x - xm;  // subtract approximate mean to avoid loss of precision in sums
    sy += y; sxy += x1 * y; sxxy += x1 * x1 * y;
    if (y < accuracy) break;}
  for (x=xm-1; x>=xmin; x--) {
    y = probability(x);
    x1 = x - xm;  // subtract approximate mean to avoid loss of precision in sums
    sy += y; sxy += x1 * y; sxxy += x1 * x1 * y;
    if (y < accuracy) break;}

  me1 = sxy / sy;
  *mean_ = me1 + xm;
  y = sxxy / sy - me1 * me1;
  if (y < 0) y=0;
  *var_ = y;
  return sy;}


double CFishersNCHypergeometric::probability(int32 x) {
  // calculate probability function
  const double accuracy = 1E-10;  // accuracy of calculation

  if (x < xmin || x > xmax) return 0;
  if (!rsum) {
    // first time. calculate rsum = reciprocal of sum of proportional 
    // function over all probable x values
    int32 x1, x2;                   // x loop
    double y;                       // value of proportional function
    x1 = (int32)mean();             // start at mean
    if (x1 < xmin) x1 = xmin;
    x2 = x1 + 1;
    scale = 0.; scale = lng(x1);    // calculate scale to avoid overflow
    rsum = 1.;                      // = exp(lng(x1)) with this scale
    for (x1--; x1 >= xmin; x1--) {
      rsum += y = exp(lng(x1));     // sum from x1 and down 
      if (y < accuracy) break;}     // until value becomes negligible
    for (; x2 <= xmax; x2++) {      // sum from x2 and up
      rsum += y = exp(lng(x2));
      if (y < accuracy) break;}     // until value becomes negligible
    rsum = 1. / rsum;}              // save reciprocal sum

  return exp(lng(x)) * rsum;}       // function value


//double CFishersNCHypergeometric::probabilityRatio(int32 x, int32 x0) {
//  // Calculate probability ratio f(x)/f(x0)
//  // This is much faster than calculating a single probability because
//  // rsum is not needed
//  double a1, a2, a3, a4, f1, f2, f3, f4;
//  int32 y, dx = x - x0;
//  int invert = 0;
//
//  if (x < xmin || x > xmax) return 0.;
//  if (x0 < xmin || x0 > xmax) FatalError("Infinity in CFishersNCHypergeometric::probabilityRatio");
//  if (dx == 0.) return 1.;
//  if (dx < 0.) {
//    invert = 1;
//    dx = -dx;
//    y = x;  x = x0;  x0 = y;}
//  a1 = m - x0;  a2 = n - x0;  a3 = x;  a4 = N - m - n + x;
//  if (dx <= 28 && x <= 100000) {       // avoid overflow
//    // direct calculation
//    f1 = f2 = 1.;
//    // compute ratio of binomials
//    for (y = 0; y < dx; y++) {
//      f1 *= a1-- * a2--;
//      f2 *= a3-- * a4--;}
//    // compute odds^dx
//    f3 = 1.;  f4 = odds;  y = dx;
//    while (y) {
//      if (f4 < 1.E-100) {
//        f3 = 0.;  break;}              // avoid underflow
//      if (y & 1) f3 *= f4;
//      f4 *= f4;
//      y = (unsigned long)(y) >> 1;}
//    f1 = f3 * f1 / f2;
//    if (invert) f1 = 1. / f1;}
//  else {
//    // use logarithms
//    f1 = FallingFactorial(a1,dx) + FallingFactorial(a2,dx) -
//         FallingFactorial(a3,dx) - FallingFactorial(a4,dx) +
//         dx * log(odds);
//    if (invert) f1 = -f1;
//    f1 = exp(f1);}
//  return f1;}
//
//
double CFishersNCHypergeometric::lng(int32 x) {
  // natural log of proportional function
  // returns lambda = log(m!*x!/(m-x)!*m2!*x2!/(m2-x2)!*odds^x)
  int32 x2 = n-x, m2 = N-m;
  if (ParametersChanged) {
    mFac = LnFac(m) + LnFac(m2);
    xLast = -99; ParametersChanged = 0;}
  if (m < FAK_LEN && m2 < FAK_LEN)  goto DEFLT;
  switch (x - xLast) {
  case 0: // x unchanged
    break;
  case 1: // x incremented. calculate from previous value
    xFac += log (double(x) * (m2-x2) / (double(x2+1)*(m-x+1)));
    break;
  case -1: // x decremented. calculate from previous value
    xFac += log (double(x2) * (m-x) / (double(x+1)*(m2-x2+1)));
    break;
  default: DEFLT: // calculate all
    xFac = LnFac(x) + LnFac(x2) + LnFac(m-x) + LnFac(m2-x2);
    }
  xLast = x;
  return mFac - xFac + x * logodds - scale;}


/***********************************************************************
     calculation methods in class CMultiFishersNCHypergeometric
***********************************************************************/
  
CMultiFishersNCHypergeometric::CMultiFishersNCHypergeometric(int32 n_, int32 * m_, double * odds_, int colors_, float accuracy_) {
  // constructor
  int32 N1;
  int i;
  // copy parameters
  n = n_;  m = m_;  odds = odds_;  colors = colors_;  accuracy = accuracy_;
  // check if parameters are valid
  for (N=N1=0, i=0; i < colors; i++) {
    if (m[i] < 0 || odds[i] < 0) FatalError("Parameter negative in constructor for CMultiFishersNCHypergeometric");
    N += m[i];
    if (odds[i]) N1 += m[i];}
  if (N < n) FatalError("Not enough items in constructor for CMultiFishersNCHypergeometric");
  if (N1< n) FatalError("Not enough items with nonzero weight in constructor for CMultiFishersNCHypergeometric");

  // calculate mFac and logodds
  for (i=0, mFac=0.; i < colors; i++) {
    mFac += LnFac(m[i]);
    logodds[i] = log(odds[i]);}

  // initialize
  sn = 0;}


void CMultiFishersNCHypergeometric::mean(double * mu) {
  // calculates approximate mean of multivariate Fisher's noncentral
  // hypergeometric distribution. Result is returned in mu[0..colors-1].
  // The calculation is reasonably fast.
  if (colors < 3) {
    // simple cases
    if (colors == 1) mu[0] = n;
    if (colors == 2) {
      mu[0] = CFishersNCHypergeometric(n,m[0],m[0]+m[1],odds[0]/odds[1]).mean();
      mu[1] = n - mu[0];}
    return;}

  double r, r1;              // iteration variable
  double q;                  // mean of color i
  double W;                  // total weight
  int i;                     // color index
  int iter = 0;              // iteration counter

  // initial guess for r
  for (i=0, W=0.; i < colors; i++) W += m[i] * odds[i];
  r = (double)n * N / ((N-n)*W);

  // iteration loop to find r
  do {
    r1 = r;
    for (i=0, q=0.; i < colors; i++) {
      q += m[i] * r * odds[i] / (r * odds[i] + 1.);}
    r *= n * (N-q) / (q * (N-n));
    if (++iter > 100) FatalError("convergence problem in function CMultiFishersNCHypergeometric::mean");}
  while (fabs(r-r1) > 1E-5);

  // store result
  for (i=0; i < colors; i++) {
    mu[i] = m[i] * r * odds[i] / (r * odds[i] + 1.);}}


void CMultiFishersNCHypergeometric::variance(double * var) {
  // calculates approximate variance of multivariate Fisher's noncentral
  // hypergeometric distribution (accuracy is not too good).
  // Result is returned in variance[0..colors-1].
  // The calculation is reasonably fast.
  double r1, r2;
  double mu[MAXCOLORS];
  int i;
  mean(mu);
  for (i=0; i<colors; i++) {
    r1 = mu[i] * (m[i]-mu[i]);
    r2 = (n-mu[i])*(mu[i]+N-n-m[i]);
    if (r1 <= 0. || r2 <= 0.) {
      var[i] = 0.;}
    else {
      var[i] = N*r1*r2/((N-1)*(m[i]*r2+(N-m[i])*r1));}}}


double CMultiFishersNCHypergeometric::probability(int32 * x) {
  // Calculate probability function.
  // Note: The first-time call takes very long time because it requires
  // a calculation of all possible x combinations with probability >
  // accuracy, which may be extreme.
  // The calculation uses logarithms to avoid overflow. 
  // (Recursive calculation may be faster, but this has not been implemented)
  if (sn == 0) SumOfAll();             // first time initialize
  return exp(lng(x)) * rsum;}          // function value


double CMultiFishersNCHypergeometric::moments(double * mean, double * variance, int32 * combinations) {
  // calculates mean and variance of the Fisher's noncentral hypergeometric 
  // distribution by calculating all combinations of x-values with
  // probability > accuracy.
  // Return value = 1.
  // Returns the mean in mean[0...colors-1]
  // Returns the variance in variance[0...colors-1]

  int i;                               // color index
  if (sn == 0) {
    // first time initialization includes calculation of mean and variance
    SumOfAll();}

  // just copy results
  for (i=0; i < colors; i++) {
    mean[i] = sx[i];
    variance[i] = sxx[i];}
  if (combinations) *combinations = sn;
  return 1.;}


void CMultiFishersNCHypergeometric::SumOfAll() {
  // this function does the very time consuming job of calculating the sum
  // of the proportional function g(x) over all possible combinations of
  // the x[i] values with probability > accuracy. These combinations are 
  // generated by the recursive function loop().
  // The mean and variance are generated as by-products.

  int i;                               // color index
  int32 msum;                          // sum of m[i]

  // get approximate mean
  mean(sx);
  // round mean to integers
  for (i=0, msum=0; i < colors; i++) {
    msum += xm[i] = (int32)(sx[i]+0.4999999);}
  // adjust truncated x values to make the sum = n
  msum -= n;
  for (i = 0; msum < 0; i++) {
    if (xm[i] < m[i]) {
      xm[i]++; msum++;}}
  for (i = 0; msum > 0; i++) {
    if (xm[i] > 0) {
      xm[i]--; msum--;}}

  // adjust scale factor to g(mean) to avoid overflow
  scale = 0.; scale = lng(xm);

  // initialize for recursive loops
  sn = 0;
  for (i = colors-1, msum = 0; i >= 0; i--) {
    remaining[i] = msum;  msum += m[i];}
  for (i = 0; i < colors; i++) {
    sx[i] = 0;  sxx[i] = 0;}

  // recursive loops to calculate sums of g(x) over all x combinations
  rsum = 1. / loop(n, 0);

  // calculate mean and variance
  for (i=0; i<colors; i++) {
    sxx[i] = sxx[i]*rsum - sx[i]*sx[i]*rsum*rsum;
    sx[i] = sx[i]*rsum;}}
    

double CMultiFishersNCHypergeometric::loop(int32 n, int c) {
  // recursive function to loop through all combinations of x-values.
  // used by SumOfAll
  int32 x, x0;                         // x of color c
  int32 xmin, xmax;                    // min and max of x[c]
  double s1, s2, sum = 0.;             // sum of g(x) values
  int i;                               // loop counter

  if (c < colors-1) {
    // not the last color
    // calculate min and max of x[c] for given x[0]..x[c-1]
    xmin = n - remaining[c];  if (xmin < 0) xmin = 0;
    xmax = m[c];  if (xmax > n) xmax = n;
    x0 = xm[c];  if (x0 < xmin) x0 = xmin;  if (x0 > xmax) x0 = xmax;
    // loop for all x[c] from mean and up
    for (x = x0, s2 = 0.; x <= xmax; x++) {
      xi[c] = x;
      sum += s1 = loop(n-x, c+1); // recursive loop for remaining colors
      if (s1 < accuracy && s1 < s2) break; // stop when values become negligible
      s2 = s1;}
    // loop for all x[c] from mean and down
    for (x = x0-1; x >= xmin; x--) {
      xi[c] = x;
      sum += s1 = loop(n-x, c+1); // recursive loop for remaining colors
      if (s1 < accuracy && s1 < s2) break; // stop when values become negligible
      s2 = s1;}}
  else {
    // last color
    xi[c] = n;
    // sums and squaresums    
    s1 = exp(lng(xi));     // proportional function g(x)
    for (i = 0; i < colors; i++) { // update sums
      sx[i]  += s1 * xi[i];
      sxx[i] += s1 * xi[i] * xi[i];}
    sn++;
    sum += s1;}
  return sum;}


double CMultiFishersNCHypergeometric::lng(int32 * x) {
  // natural log of proportional function g(x)
  double y = 0.;
  int i;
  for (i=0; i < colors; i++) {
    y += x[i]*logodds[i] - LnFac(x[i]) - LnFac(m[i]-x[i]);}
  return mFac + y - scale;}
