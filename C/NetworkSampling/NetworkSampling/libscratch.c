/*******************************/
/* DRAFT NOT READY FOR RELEASE */
/*******************************/
/* libscratch:  A library of C functions for network sampling estimation.
 Copyright 2020 Steve Thompson under the Gnu General Public License.
 **************************************************************************
 time gcc -c -Wall -Ofast -funroll-loops `sdl-config --cflags --libs` -lSDL_gfx -o libdraw.o libdraw.c -lm
 */

//#include <unistd.h>  /* Many POSIX functions (but not all, by a large margin) */
#include <fcntl.h>   /* open(), creat() - and fcntl() */
#include "scratch.h"
#include "stdafx.h"


/*********************************************************************
 *********************************************************************

   Linked-list functions 

 *********************************************************************
 *********************************************************************/

NODE first = NULL;
extern long long int randomcalls; 

NODE createnode()
{
  NODE alloc;
  alloc = (NODE) malloc(sizeof(struct node));
  if (alloc == NULL)
    {
      printf("memory for node can not be allocated\n");
      exit(0);
    }
  return(alloc);
}
EDGENODE createedgenode()
{
  EDGENODE alloc;
  alloc = (EDGENODE) malloc(sizeof(struct edgenode));
  if (alloc == NULL)
    {
      printf("memory for edge node can not be allocated\n");
      exit(0);
    }
  return(alloc);
}

/* insert new node at front, does not order list */
void insert_front(int item)
{
  NODE tmp;
  int i;
  tmp = createnode();
  tmp->id = item;
  tmp->degree = 0;
  tmp->coordx = 0.5;
  tmp->coordy = 0.5;
  tmp->bug = 0;
  tmp->aadlagav = 0.0;
  tmp->fi = 0;
  for(i=0; i < 2; i++) tmp->aad[i] = 0;
  tmp->datavar = NULL; 
  tmp -> next = NULL;
  tmp->adjfirst = NULL;
  if (first == NULL)
    {
      first = tmp;
    }
  else
    {
      tmp->next = first;
      first = tmp;
    }
} /* end of insert front */

/* inserts an edge from node with id u to node with id v 
   in an adjacency list */
void insert_edge( int u, int v )
{ 
  NODE locu, locv; 
  EDGENODE ptr, tmp;;
  locu = find( u ); /* find address in memory of node u */
  locv = find( v ); /* find address in memory of node v */
    if ( locu == NULL )
        {
            printf( "source node u %d not present\n", u );
            return ;
        }
    if ( locv == NULL )
        {
	  printf( "destination node v %d not present\n", v );
	  return ;
        }

    /* if u has no edges yet, ad v at start of edge list from u */
    if ( locu->adjfirst == NULL )  
      {
	tmp = createedgenode();
	tmp->id = v;
	tmp->tracing = -30;
	tmp->idealizedtraced = 0;
	tmp->fij = 0;
	tmp->edgedegree = 0;;
	tmp->linkvar = NULL; /* for simple estimation */  
	tmp->idaddress = locv; /* pointer to destination node */
	tmp->adj = NULL;
	locu->adjfirst = tmp;
	return ;
      }
    if ( locu->adjfirst->id == v) /* if v already there at start */
      {
	return;
      }

    /* go through edge list from u to see if v is already there */
      ptr = locu->adjfirst;
    while ( ptr->adj != NULL )
      {
	if (ptr->adj->id == v) /* if already there */
	  {
	    return;
	  }
        ptr = ptr->adj;
      }
    tmp = createedgenode();
    tmp->id = locv->id;
    tmp->tracing = -30;
    tmp->idealizedtraced = 0;
    tmp->fij = 0;
    tmp->edgedegree = 0; /* i think i will make this edge out-degree */
    tmp->linkvar = NULL; /* for simple estimation */  
    tmp->idaddress = locv; /* pointer to destination node */
    tmp->adj = NULL;
    ptr->adj = tmp; /* add v at end of adjacency list from u */ 

} /*End of insert_edge()*/

/* find a node by its id and return pointer to it */
struct node *find( int item )
    {
        struct node *ptr, *loc;
        ptr = first;
        while ( ptr != NULL )
            {
                if ( item == ptr->id )
                    {
                        loc = ptr;
                        return loc;
                    }
                else
                    ptr = ptr->next;
            } 
        loc = NULL;
        return loc;
    } /*End of find()*/


/****************************
 RANDOM NUMBERS 
*****************************/

/******* these functions are for Mersenne Twister *********/

/* http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/emt19937ar.html

   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)  
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

/****************
 I am modifying the original code to have the current state
of the mersenne twister in the function call so that independent
streams of random numbers can be produced 
******************/

/* see also the gsl function file mt.c by Brian Gough with the
following copyright and notes */
/* This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.  You should have received
   a copy of the GNU General Public License along with this library; if
   not, write to the Free Software Foundation, Inc., 51 Franklin Street,
   Fifth Floor, Boston, MA 02110-1301, USA.

   Original implementation was copyright (C) 1997 Makoto Matsumoto and
   Takuji Nishimura. Coded by Takuji Nishimura, considering the
   suggestions by Topher Cooper and Marc Rieffel in July-Aug. 1997, "A
   C-program for MT19937: Integer version (1998/4/6)"

   This implementation copyright (C) 1998 Brian Gough. I reorganized
   the code to use the module framework of GSL.  The license on this
   implementation was changed from LGPL to GPL, following paragraph 3
   of the LGPL, version 2.

   Update:

   The seeding procedure has been updated to match the 10/99 release
   of MT19937.

   Update:

   The seeding procedure has been updated again to match the 2002
   release of MT19937

   The original code included the comment: "When you use this, send an
   email to: matumoto@math.keio.ac.jp with an appropriate reference to
   your work".

   Makoto Matsumoto has a web page with more information about the
   generator, http://www.math.keio.ac.jp/~matumoto/emt.html. 

   The paper below has details of the algorithm.

   From: Makoto Matsumoto and Takuji Nishimura, "Mersenne Twister: A
   623-dimensionally equidistributerd uniform pseudorandom number
   generator". ACM Transactions on Modeling and Computer Simulation,
   Vol. 8, No. 1 (Jan. 1998), Pages 3-30

   You can obtain the paper directly from Makoto Matsumoto's web page.

   The period of this generator is 2^{19937} - 1.

*/


/* Period parameters */  
/* #define NN 624 */
/* #define M 397 */
/* #define MATRIX_A 0x9908b0dfUL   /\* constant vector a *\/ */
/* #define UPPER_MASK 0x80000000UL /\* most significant w-r bits *\/ */
/* #define LOWER_MASK 0x7fffffffUL /\* least significant r bits *\/ */

/* struct mtstate  */
/* { */
/*   unsigned long mt[NN]; /\* the array for the state vector  *\/ */
/*   int mti;  /\*mti=NN+1;  mti==N+1 means mt[N] is not initialized *\/ */
/* }; */

struct mtstate *createmtstate()
{
  struct mtstate *alloc;
  alloc = (struct mtstate *) malloc(sizeof(struct mtstate));
  if (alloc == NULL)
    {
      printf("memory for Mersenne Twister state can not be allocated\n");
      exit(0);
    }
  return(alloc);
}


/* initializes mt[NN] using a single seed s*/
void init_genrand(struct mtstate *mts, unsigned long s)
{
  /* printf("Got into init_genrand\n"); */
  int i;
  mts->mt[0]= s & 0xffffffffUL;
    for (i=1; i<NN; i++) {
        mts->mt[i] = 
	    (1812433253UL * (mts->mt[i-1] ^ (mts->mt[i-1] >> 30)) + i); 
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mts->mt[i] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
    mts->mti = i;
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
void init_by_array(struct mtstate *mts, unsigned long init_key[], int key_length)
{
    int i, j, k;
  printf("Got into init_by array\n");
  init_genrand(mts, 19650218UL);
    i=1; j=0;
    k = (NN>key_length ? NN : key_length);
    for (; k; k--) {
        mts->mt[i] = (mts->mt[i] ^ ((mts->mt[i-1] ^ (mts->mt[i-1] >> 30)) * 1664525UL))
          + init_key[j] + j; /* non linear */
        mts->mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++; j++;
        if (i>=NN) { mts->mt[0] = mts->mt[NN-1]; i=1; }
        if (j>=key_length) j=0;
    }
    for (k=NN-1; k; k--) {
        mts->mt[i] = (mts->mt[i] ^ ((mts->mt[i-1] ^ (mts->mt[i-1] >> 30)) * 1566083941UL))
          - i; /* non linear */
        mts->mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++;
        if (i>=NN) { mts->mt[0] = mts->mt[NN-1]; i=1; }
    }

    mts->mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
}

/* generates a random number on [0,0xffffffff]-interval */
static inline unsigned long genrand_int32(struct mtstate *mts)
{
    unsigned long y;
    unsigned long int *const mt = mts->mt;
    unsigned long mag01[2]={0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mts->mti >= NN) { /* generate N words at one time */
        int kk;

        if (mts->mti == NN+1)   /* if init_genrand() has not been called, */
	  {
            init_genrand(mts, 5489UL); /* a default initial seed is used */
	  }

        for (kk=0;kk<NN-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<NN-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-NN)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[NN-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[NN-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mts->mti = 0;
    }
  
    y = mt[mts->mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

/* generates a random number on [0,0x7fffffff]-interval */
long genrand_int31(struct mtstate *mts)
{
    return (long)(genrand_int32(mts)>>1);
}

/* generates a random number on [0,1]-real-interval */
double genrand_real1(struct mtstate *mts)
{
    return genrand_int32(mts)*(1.0/4294967295.0); 
    /* divided by 2^32-1 */ 
}

/* generates a random number on [0,1)-real-interval */
double genrand_real2(struct mtstate *mts)
{
    return genrand_int32(mts)*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
double genrand_real3(struct mtstate *mts)
{
    return (((double)genrand_int32(mts)) + 0.5)*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on [0,1) with 53-bit resolution*/
double genrand_res53(struct mtstate *mts) 
{ 
    unsigned long a=genrand_int32(mts)>>5, b=genrand_int32(mts)>>6; 
    return(a*67108864.0+b)*(1.0/9007199254740992.0); 
} 
/* These real versions are due to Isaku Wada, 2002/01/09 added */

/******* end of m. twister functions *********/

/* Seed from hardware /dev/urandom */
/* see http://www0.cs.ucl.ac.uk/staff/d.jones/GoodPracticeRNG.pdf 
 */
unsigned int devrand(void)
{
  int fn;
  unsigned int r;
  fn = open("/dev/urandom", O_RDONLY);
  if (fn == -1)
    exit(-1); /* Failed! */
  if (read(fn, &r, 4) != 4)
    exit(-1); /* Failed! */
  close(fn);
  /* printf("seed from hardware device\n"); */
  return r;
}


/****** function for KISS random number generator, Jones version ******/
/* I made some modifications and put these in my library.  But
   since it did not run faster that mersenne twister i took it out */

/* Public domain code for JKISS RNG */
static unsigned int xkiss = 123456789,ykiss = 987654321,zkiss = 43219876,ckiss = 6543217; /* Seed variables */
unsigned int JKISS()
{
  unsigned long long t;
  xkiss = 314527869 * xkiss + 1234567;
  ykiss ^= ykiss << 5; ykiss ^= ykiss >> 7; ykiss ^= ykiss << 22;
  t = 4294584393ULL * zkiss + ckiss; ckiss = t >> 32; zkiss = t;
  return xkiss + ykiss + zkiss;
}

/* Initialise KISS generator using /dev/urandom */
void init_KISS()
{
xkiss = devrand();
while (!(ykiss = devrand())); /* y must not be zero! */
zkiss = devrand();
/* We don’t really need to set c as well but let's anyway... */
/* NOTE: offset c by 1 to avoid z=c=0 */
ckiss = devrand() % 698769068 + 1; /* Should be less than 698769069 */
}

/* Implementation of a 32-bit KISS generator which uses no multiply instructions */
static unsigned int wkiss=456789123;
unsigned int JKISS32()
{
int t;
ykiss ^= (ykiss<<5); ykiss ^= (ykiss>>7); ykiss ^= (ykiss<<22);
t = zkiss+wkiss+ckiss; zkiss = wkiss; ckiss = t < 0; wkiss = t&2147483647;
xkiss += 1411392427;
return xkiss + ykiss + wkiss;
}

/* Uniform [0,1) random variable */
double runif(struct mtstate *mts)
{
  /* randomcalls = randomcalls + 1; */
  /* return 1.0 * rand()/((double) RAND_MAX + 1); */ /* stdlib rand() */
  return genrand_real2(mts); /* Mersenne twister */
  /* return 1.0 * JKISS() / 4294967296.0; */ /* kiss rng, Jones' version */
  /* return 1.0 * JKISS32() / 4294967296.0; */ /* shorter period, Jones' version */
}

/* Uniform [0,1), uses Mersenne twister */
double runifmtwist(struct mtstate *mts)
{
  randomcalls = randomcalls + 1;
  return genrand_real2(mts);
}

/* for kiss rng, Jones' version */
double runifjkiss()
{
  randomcalls = randomcalls + 1;
  return 1.0 * JKISS() / 4294967296.0;
}

/* this one uses the glibc rand() in stdlib.h */
/* runif for U(0,1), can try any number of ways here */
double runifrand()
{
  return 1.0 * rand()/((double) RAND_MAX + 1);
  /* return drand48(); */
}


/* sort array of doubles in descending order */
/* comparision function for qsort */
int compare(const void *aIn, const void *bIn)
{
  double a = *((double*)aIn);
  double b = *((double*)bIn);

  if(a > b)
    return -1;
  else if(a < b)
    return 1;
  return 0;		      
}

#define TRUE 1

/*        this function will get a string array
	  and split into the words, seprated by spaces  */
/* ten words of maximum length 20 */ 

int getWords(char *base, char target[10][20])
{
	int n=0,i,j=0;
	
	for(i=0;TRUE;i++)
	{
		if(base[i]!=' '){
			target[n][j++]=base[i];
		}
		else{
			target[n][j++]='\0';//insert NULL
			n++;
			j=0;
		}
		if(base[i]=='\0') /* end of line */
		  {
		    n = n + 1;  /* count last word in line */
		    break;
		  }
	}
	return n; /* n is the number of words in the line */
}
