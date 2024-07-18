/***************************** STOCC.H ************************ 2004-01-08 AF *
*
* This file contains function prototypes and class declarations for the C++ 
* library of non-uniform random number generators. Most functions are fast and 
* accurate, even for extreme values of the parameters.
*
*
* functions without classes:
* ==========================
*
* void EndOfProgram(void);
* System-specific exit code. You may modify this to make it fit your
* user interface.
*
* void FatalError(char * ErrorText);
* Used for outputting error messages from the other functions and classes.
* You may have to modify this function to make it fit your user interface.
*
* double Erf (double x);
* Calculates the error function, which is the integral of the normal distribution.
*
* double LnFac(int32 n);
* Calculates the natural logarithm of the factorial of n.
*
*
* class StochasticLib1:
* ====================
* This class can be derived from any of the uniform random number generators
* defined in randomc.h. StochasticLib1 provides the following non-uniform random 
* variate generators:
*
* int Bernoulli(double p);
* Bernoulli distribution. Gives 0 or 1 with probability 1-p and p.
*
* double Normal(double m, double s);
* Normal distribution with mean m and standard deviation s.
*
* int32 Poisson (double L);
* Poisson distribution with mean L.
*
* int32 Binomial (int32 n, double p);
* Binomial distribution. n trials with probability p.
*
* int32 Hypergeometric (int32 n, int32 m, int32 N);
* Hypergeometric distribution. Taking n items out N, m of which are colored.
*
* void Multinomial (int32 * destination, double * source, int32 n, int colors);
* void Multinomial (int32 * destination, int32 * source, int32 n, int colors);
* Multivariate binomial distribution.
*
* void MultiHypergeometric (int32 * destination, int32 * source, int32 n, int colors);
* Multivariate hypergeometric distribution.
*
* void Shuffle(int * list, int min, int n);
* Shuffle a list of integers.
*
*
* class StochasticLib2:
* =====================
* This class is derived from class StochasticLib1. It redefines the functions
* Poisson, Binomial and HyperGeometric.
* In StochasticLib1, these functions are optimized for being called with 
* parameters that vary. In StochasticLib2, the same functions are optimized
* for being called repeatedly with the same parameters. If your parameters
* seldom vary, then StochasticLib2 is faster. The two classes use different
* calculation methods, both of which are accurate.
*
*
* class StochasticLib3:
* =====================
* This class can be derived from either StochasticLib1 or StochasticLib2, 
* whichever is preferred. It contains functions for generating variates with
* the univariate and multivariate Wallenius' and Fisher's noncentral
* hypergeometric distributions.
*
* int32 WalleniusNCHyp (int32 n, int32 m, int32 N, double odds);
* Sampling from Wallenius' noncentral hypergeometric distribution, which is 
* what you get when taking n items out N, m of which are colored, without 
* replacement, with bias.
*
* int32 FishersNCHyp (int32 n, int32 m, int32 N, double odds);
* Sampling from Fisher's noncentral hypergeometric distribution which is the
* conditional distribution of independent binomial variates given their sum n.
*
* void MultiWalleniusNCHyp (int32 * destination, int32 * source, double * weights, int32 n, int colors);
* Sampling from multivariate Wallenius' noncentral hypergeometric distribution.
*
* void MultiFishersNCHyp (int32 * destination, int32 * source, double * weights, int32 n, int colors);
* Sampling from multivariate Fisher's noncentral hypergeometric distribution.
*
*
* Uniform random number generators (integer and float) are also available, as
* these are inherited from the random number generator class that is the base
* class of StochasticLib1.
*
*
* class CWalleniusNCHypergeometric
* ================================
* This class implements various methods for calculating the probability 
* function and the mean and variance of the univariate Wallenius' noncentral 
* hypergeometric distribution. It is used by StochasticLib3 and can also be 
* used independently.
*
*
* class CMultiWalleniusNCHypergeometric
* =====================================
* This class implements various methods for calculating the probability func-
* tion and the mean of the multivariate Wallenius' noncentral hypergeometric
* distribution. It is used by StochasticLib3 and can also be used independently.
*
*
* class CMultiWalleniusNCHypergeometricMoments
* ============================================
* This class calculates the exact mean and variance of the multivariate
* Wallenius' noncentral hypergeometric probability distribution.
*
*
* class CFishersNCHypergeometric
* ==============================
* This class calculates the probability function and the mean and variance 
* of Fisher's noncentral hypergeometric distribution.
*
*
* class CMultiFishersNCHypergeometric
* ===================================
* This class calculates the probability function and the mean and variance 
* of the multivariate Fisher's noncentral hypergeometric distribution.
*
*
* source code:
* ============
* The code for EndOfProgram and FatalError is found in the file userintf.cpp.
* The code for the functions in StochasticLib1 is found in the file stoc1.cpp.
* The code for the functions in StochasticLib2 is found in the file stoc2.cpp.
* The code for the functions in StochasticLib3 is found in the file stoc3.cpp.
* The code for the functions in CWalleniusNCHypergeometric, 
* CMultiWalleniusNCHypergeometric and CMultiWalleniusNCHypergeometricMoments
* is found in the file wnchyppr.cpp.
* The code for the functions in CFishersNCHypergeometric and 
* CMultiFishersNCHypergeometric is found in the file fnchyppr.cpp
* LnFac is found in stoc1.cpp.
* Erf is found in wnchyppr.cpp.
*
*
* Examples:
* =========
* The file ex-stoc.cpp contains an example of how to use this class library.
*
* The file ex-cards.cpp contains an example of how to shuffle a list of items.
*
* The file ex-lotto.cpp contains an example of how to generate a sequence of
* random integers where no number can occur more than once.
*
* The file testbino.cpp contains an example of sampling from the binomial distribution.
*
* The file testhype.cpp contains an example of sampling from the hypergeometric distribution.
*
* The file testpois.cpp contains an example of sampling from the poisson distribution.
*
* The file testwnch.cpp contains an example of sampling from Wallenius noncentral hypergeometric distribution.
*
* The file testfnch.cpp contains an example of sampling from Fisher's noncentral hypergeometric distribution.
*
* The file testmwnc.cpp contains an example of sampling from the multivariate Wallenius noncentral hypergeometric distribution.
*
* The file testmfnc.cpp contains an example of sampling from the multivariate Fisher's noncentral hypergeometric distribution.
*
* The file evolc.zip contains examples of how to simulate biological evolution using this class library.
*
*
* Documentation:
* ==============
* The file stocc.htm contains further instructions.
*
* The file distrib.pdf contains definitions of the standard statistic distributions:
* Bernoulli, Normal, Poisson, Binomial, Hypergeometric, Multinomial, MultiHypergeometric.
*
* The file sampmet.pdf contains theoretical descriptions of the methods used
* for sampling from these distributions.
*
* The file nchyp.pdf, available from www.agner.org/random/, contains
* definitions of the univariate and multivariate Wallenius and Fisher's 
* noncentral hypergeometric distributions and theoretical explanations of 
* the methods for calculating and sampling from these.
*
* ?2002, 2004 Agner Fog. GNU General Public License www.gnu.org/copyleft/gpl.html
*******************************************************************************/

#ifndef STOCC_H
#define STOCC_H

#include "randomc.h"

/***********************************************************************
 Choose which uniform random number generator to base these classes on
***********************************************************************/

#ifndef RANDOM_GENERATOR
// define which random number generator to base stochastic library on:
#define RANDOM_GENERATOR TRandomMersenne
//#define RANDOM_GENERATOR TRanrotWGenerator
//#define RANDOM_GENERATOR TRandomMotherOfAll
#endif

/***********************************************************************
         System-specific user interface functions
***********************************************************************/

void EndOfProgram(void);               // system-specific exit code

void FatalError(char * ErrorText);     // system-specific error reporting

/***********************************************************************
         Other simple functions
***********************************************************************/

double Erf (double x);                 // error function
double LnFac(int32 n);              // log factorial

/***********************************************************************
         Constants and tables
***********************************************************************/

// constant for LnFac function:
static const int FAK_LEN = 1024;       // length of factorial table

// The following tables are tables of residues of a certain expansion
// of the error function. These tables are used in the Laplace method
// for calculating Wallenius' noncentral hypergeometric distribution.
// There are ERFRES_N tables covering desired precisions from
// 2^(-ERFRES_B) to 2^(-ERFRES_E). Only the table that matches the
// desired precision is used.

// constants for ErfRes tables:
static const int ERFRES_B = 16;        // begin: -log2 of lowest precision
static const int ERFRES_E = 40;        // end:   -log2 of highest precision
static const int ERFRES_S =  2;        // step size from begin to end
static const int ERFRES_N = (ERFRES_E-ERFRES_B)/ERFRES_S+1; // number of tables
static const int ERFRES_L = 48;        // length of each table

// tables of error function residues:
extern "C" double ErfRes [ERFRES_N][ERFRES_L];

// number of std. deviations to include in integral to obtain desired precision:
extern "C" double NumSDev[ERFRES_N];


/***********************************************************************
         Class StochasticLib1
***********************************************************************/

class StochasticLib1 : public RANDOM_GENERATOR {
  // This class encapsulates the random variate generating functions.
  // May be derived from any of the random number generators.
  public:
  StochasticLib1 (int seed);           // constructor
  int Bernoulli(double p);             // bernoulli distribution
  double Normal(double m, double s);   // normal distribution
  int32 Poisson (double L);            // poisson distribution
  int32 Binomial (int32 n, double p);  // binomial distribution
  int32 Hypergeometric (int32 n, int32 m, int32 N); // hypergeometric distribution
  void Multinomial (int32 * destination, double * source, int32 n, int colors); // multinomial distribution
  void Multinomial (int32 * destination, int32 * source, int32 n, int colors); // multinomial distribution
  void MultiHypergeometric (int32 * destination, int32 * source, int32 n, int colors); // multivariate hypergeometric distribution
  void Shuffle(int * list, int min, int n); // shuffle integers
  
  // functions used internally
  protected:
  static double fc_lnpk(int32 k, int32 N_Mn, int32 M, int32 n); // used by Hypergeometric

  // subfunctions for each approximation method
  int32 PoissonInver(double L);                       // poisson by inversion
  int32 PoissonRatioUniforms(double L);               // poisson by ratio of uniforms
  int32 PoissonLow(double L);                         // poisson for extremely low L
  int32 BinomialInver (int32 n, double p);            // binomial by inversion
  int32 BinomialRatioOfUniforms (int32 n, double p);  // binomial by ratio of uniforms
  int32 HypInversionMod (int32 n, int32 M, int32 N);  // hypergeometric by inversion searching from mode
  int32 HypRatioOfUnifoms (int32 n, int32 M, int32 N);// hypergeometric by ratio of uniforms method

  // variables used by Normal distribution
  double normal_x2;  int normal_x2_valid;

  // define constants
  enum constants {
    // maximum value of 'colors' in multivariate distributions:
    MAXCOLORS = 20};         // you may change this number
  };


/***********************************************************************
         Class StochasticLib2
***********************************************************************/

class StochasticLib2 : public StochasticLib1 {
  // derived class, redefining some functions
  public:
  int32 Poisson (double L);                           // poisson distribution
  int32 Binomial (int32 n, double p);                 // binomial distribution
  int32 Hypergeometric (int32 n, int32 M, int32 N);   // hypergeometric distribution
  StochasticLib2(int seed):StochasticLib1(seed){};    // constructor  
  
  // subfunctions for each approximation method:
  protected:
  int32 PoissonModeSearch(double L);                  // poisson by search from mode
  int32 PoissonPatchwork(double L);                   // poisson by patchwork rejection
  static double PoissonF(int32 k, double l_nu, double c_pm); // used by PoissonPatchwork
  int32 BinomialModeSearch(int32 n, double p);        // binomial by search from mode
  int32 BinomialPatchwork(int32 n, double p);         // binomial by patchwork rejection
  double BinomialF(int32 k, int32 n, double l_pq, double c_pm); // used by BinomialPatchwork
  int32 HypPatchwork (int32 n, int32 M, int32 N);     // hypergeometric by patchwork rejection
};


/***********************************************************************
         Class StochasticLib3
***********************************************************************/

class StochasticLib3 : public StochasticLib1 {
  // This class can be derived from either StochasticLib1 or StochasticLib2.
  // Adds more probability distributions
  public:
  StochasticLib3(int seed); // constructor
  void SetAccuracy(float accur);  // define accuracy of calculations
  int32 WalleniusNCHyp (int32 n, int32 m, int32 N, double odds); // Wallenius noncentral hypergeometric distribution
  int32 FishersNCHyp (int32 n, int32 m, int32 N, double odds); // Fisher's noncentral hypergeometric distribution
  void MultiWalleniusNCHyp (int32 * destination, int32 * source, double * weights, int32 n, int colors); // multivariate Wallenius noncentral hypergeometric distribution
  void MultiComplWalleniusNCHyp (int32 * destination, int32 * source, double * weights, int32 n, int colors); // multivariate complementary Wallenius noncentral hypergeometric distribution
  void MultiFishersNCHyp (int32 * destination, int32 * source, double * weights, int32 n, int colors); // multivariate Fisher's noncentral hypergeometric distribution
  // subfunctions for each approximation method
  protected:
  int32 WalleniusNCHypUrn (int32 n, int32 m, int32 N, double odds); // WalleniusNCHyp by urn model
  int32 WalleniusNCHypInversion (int32 n, int32 m, int32 N, double odds); // WalleniusNCHyp by inversion method
  int32 WalleniusNCHypTable (int32 n, int32 m, int32 N, double odds); // WalleniusNCHyp by table method
  int32 WalleniusNCHypRatioOfUnifoms (int32 n, int32 m, int32 N, double odds); // WalleniusNCHyp by ratio-of-uniforms
  int32 FishersNCHypInversion (int32 n, int32 m, int32 N, double odds); // FishersNCHyp by inversion
  int32 FishersNCHypRatioOfUnifoms (int32 n, int32 m, int32 N, double odds); // FishersNCHyp by ratio-of-uniforms
  // variables
  float accuracy; // desired accuracy of calculations
};


/***********************************************************************
         Class CWalleniusNCHypergeometric
***********************************************************************/

class CWalleniusNCHypergeometric {
  // This class contains methods for calculating the univariate
  // Wallenius' noncentral hypergeometric probability function
  public:
  CWalleniusNCHypergeometric(int32 n, int32 m, int32 N, double odds, float accuracy=1.E-8); // constructor
  void SetParameters(int32 n, int32 m, int32 N, double odds); // change parameters
  double probability(int32 x); // calculate probability function
  int MakeTable(double * table, int32 MaxLength, int32 * start, int32 * end); // make table of probabilities
  double mean(void);         // approximate mean
  double variance(void);     // approximate variance (poor approximation)
  double moments(double * mean, double * var); // calculate exact mean and variance
  int BernouilliH(int32 x, double h, double rh, StochasticLib1 *sto); // used by rejection method

  // implementations of different calculation methods
  protected:
  double recursive(void);    // recursive calculation
  double binoexpand(void);   // binomial expansion of integrand
  double laplace(void);      // Laplace's method with narrow integration interval
  double integrate(void);    // numerical integration

  // other subfunctions
  double lnbico(void);       // natural log of binomial coefficients
  void findpars(void);       // calculate r, w, E
  double integrate_step(double a, double b); // used by integrate()
  double search_inflect(double t_from, double t_to); // used by integrate()

  // parameters
  double omega;
  int32 n, m, N, x;
  int32 xmin, xmax;
  float accuracy;
  // parameters used by lnbico
  int32 xLastBico;
  double bico, mFac, xFac;
  // parameters generated by findpars and used by probability, laplace, integrate:
  double r, rd, w, wr, E, phi2d;
  int32 xLastFindpars;
  };


/***********************************************************************
         Class CMultiWalleniusNCHypergeometric
***********************************************************************/

class CMultiWalleniusNCHypergeometric {
  // This class encapsulates the different methods for calculating the
  // multivariate Wallenius noncentral hypergeometric probability function
  public:
  enum constants {
    MAXCOLORS = 20};                   // maximum number of colors. May be changed
  CMultiWalleniusNCHypergeometric(int32 n, int32 * m, double * odds, int colors, float accuracy=1.E-8f); // constructor
  void SetParameters(int32 n, int32 * m, double * odds, int colors); // change parameters
  double probability(int32 * x);       // calculate probability function
  void mean(double * mu);              // calculate approximate mean

  // implementations of different calculation methods
  protected:
  double binoexpand(void);   // binomial expansion of integrand
  double laplace(void);      // Laplace's method with narrow integration interval
  double integrate(void);    // numerical integration

  // other subfunctions
  double lnbico(void);       // natural log of binomial coefficients
  void findpars(void);       // calculate r, w, E
  double integrate_step(double a, double b); // used by integrate()
  double search_inflect(double t_from, double t_to); // used by integrate()

  // parameters
  double * omega;
  float accuracy;
  int32 n, N;
  int32 * m, * x;
  int colors;
  int Dummy_align;
  // parameters generated by findpars and used by probability, laplace, integrate:
  double r, rd, w, wr, E, phi2d;
  // generated by lnbico
  double bico;
  };


/***********************************************************************
         Class CMultiWalleniusNCHypergeometricMoments
***********************************************************************/

class CMultiWalleniusNCHypergeometricMoments: public CMultiWalleniusNCHypergeometric {
  // This class calculates the exact mean and variance of the multivariate
  // Wallenius noncentral hypergeometric distribution by calculating all the 
  // possible x-combinations with probability < accuracy
  public:
  CMultiWalleniusNCHypergeometricMoments(int32 n, int32 * m, double * odds, int colors, float accuracy=1.E-8f) 
  : CMultiWalleniusNCHypergeometric(n, m, odds, colors, accuracy) {};
  double moments(double * mean, double * stddev, int32 * combinations = 0);

  protected:
  // functions used internally
  double loop(int32 n, int c);         // recursive loops
  // data
  int32 xi[MAXCOLORS];                 // x vector to calculate probability of
  int32 xm[MAXCOLORS];                 // rounded approximate mean of x[i]
  int32 remaining[MAXCOLORS];          // number of balls of color > c in urn
  double sx[MAXCOLORS];                // sum of x*f(x)
  double sxx[MAXCOLORS];               // sum of x^2*f(x)
  int32 sn;                            // number of combinations
  };


/***********************************************************************
         Class CFishersNCHypergeometric
***********************************************************************/

class CFishersNCHypergeometric {
  // This class contains methods for calculating the univariate Fisher's
  // noncentral hypergeometric probability function
  public:
  CFishersNCHypergeometric(int32 n, int32 m, int32 N, double odds); // constructor
  double probability(int32 x);                   // calculate probability function
  //double probabilityRatio(int32 x, int32 x0);    // calculate probability f(x)/f(x0)
  double mean(void);                             // calculate approximate mean
  double moments(double * mean, double * var);   // calculate exact mean and variance

  protected:
  double lng(int32 x);                           // natural log of proportional function

  // parameters
  double odds;                         // odds ratio
  double logodds;                      // ln odds ratio
  int32 n, m, N;
  int32 xmin, xmax;                    // minimum and maximum of x

  // parameters used by subfunctions
  int32 xLast;
  double mFac, xFac;                   // log factorials
  double scale;                        // scale to apply to lng function
  double rsum;                         // reciprocal sum of proportional function
  int ParametersChanged;
  };


/***********************************************************************
         Class CMultiFishersNCHypergeometric
***********************************************************************/

class CMultiFishersNCHypergeometric {
  // This class contains functions for calculating the multivariate
  // Fisher's noncentral hypergeometric probability function and its mean and 
  // variance. Warning: the time consumption for first call to 
  // probability or moments is proportional to the total number of
  // possible x combinations, which may be extreme!
  public:
  enum constants {
    MAXCOLORS = 16};                   // maximum number of colors. May be changed
  CMultiFishersNCHypergeometric(int32 n, int32 * m, double * odds, int colors, float accuracy = 1E-9f); // constructor
  double probability(int32 * x);       // calculate probability function
  void mean(double * mu);              // calculate approximate mean
  void variance(double * var);         // calculate approximate variance
  double moments(double * mean, double * stddev, int32 * combinations = 0); // calculate exact mean and variance

  protected:
  double lng(int32 * x);               // natural log of proportional function
  void SumOfAll(void);                 // calculates sum of proportional function for all x combinations
  double loop(int32 n, int c);         // recursive loops used by SumOfAll
  int32 n, N;                          // copy of parameters
  int32 * m;
  double * odds;
  int colors;
  double logodds[MAXCOLORS];           // log odds
  double mFac;                         // sum of log m[i]!
  double scale;                        // scale to apply to lng function
  double rsum;                         // reciprocal sum of proportional function
  float accuracy;                      // accuracy of calculation

  // data used by used by SumOfAll
  int32 xi[MAXCOLORS];                 // x vector to calculate probability of
  int32 xm[MAXCOLORS];                 // rounded approximate mean of x[i]
  int32 remaining[MAXCOLORS];          // number of balls of color > c in urn
  double sx[MAXCOLORS];                // sum of x*f(x) or mean
  double sxx[MAXCOLORS];               // sum of x^2*f(x) or variance
  int32 sn;                            // number of possible combinations of x
  };

#endif

