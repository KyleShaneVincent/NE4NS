/*******************************/
/* DRAFT NOT READY FOR RELEASE */
/*******************************/
/* scratch.h, header file to go with scratchx.c,  Network Sampling Estimation.
 Copyright 2020 Steve Thompson under the Gnu General Public License.
**************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <tgmath.h>
#include <time.h>
#include "stdafx.h"


struct edgenode
{
  int id;
  int idealizedtraced; /* indicator link has been traced in idealized */
  int fij; /* number of inclusions of edge (i,j) in fast design */
  int edgedegree; /* number of links adjacent to this one */
  int *linkvar; /* to estimate mean of link variable of choice, array later*/
  int tracing;
  struct node *idaddress;
  struct edgenode *adj;
};
struct node
{
  int id;
  int degree;
  double coordx;
  double coordy;
  int bug;
  double aadlagav;
  int found[2]; /* inclusion in finding design */
  int coupon; /* idealized design coupons */
  int fi; /* number of inclusions of unit i in fast design */
  int aad[2]; /* inclusion in fast sample, current aad[0], past aad[1] */
  int * datavar; /* all-purpuse variable i read in from data for estimates */
  struct node *next;
  struct edgenode *adjfirst;
};
typedef struct node *NODE;
typedef struct edgenode *EDGENODE;

/* mersenne twister, random number generator */
/* Period parameters */  
#define NN 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

/* state of marsenne twister */
struct mtstate 
{
  unsigned long mt[NN]; /* the array for the state vector  */
  int mti;  /* mti=NN+1;  mti==N+1 means mt[N] is not initialized */
};


NODE createnode();

EDGENODE createedgenode();

struct mtstate *createmtstate();

/* insert new node at front, does not order list */
void insert_front(int item);

/* inserts an edge from node with id u to node with id v */
  void insert_edge(int u, int v);

/* find a node by its id and return pointer to it */
struct node *find( int item );

/* random variable generation */

/*** marsenne twister random number generation ***/

/* initializes mt[NN] with a seed */
void init_genrand(struct mtstate *mts, unsigned long s);

/*** end marsenne twister functions ***/

/* get seed from hardware /dev/random */
unsigned int devrand(void);

/* Initialise KISS generator using /dev/urandom */
void init_KISS();

/* runif for U(0,1) using m twister */
double runif(struct mtstate *mts);

/* sort array of doubles in descending order */
/* comparision function for qsort */
int compare(const void *aIn, const void *bIn);

/* this function will get a string array
   and split into the words, seprated by spaces  */
/* ten words of maximum length 20 */
int getWords(char *base, char target[10][20]);
