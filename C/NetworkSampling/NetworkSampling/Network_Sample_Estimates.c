/*******************************/
/* DRAFT NOT READY FOR RELEASE */
/**********************************************************************/
/* Copyright 2020 Steve Thompson under the Gnu General Public License.*/
/**********************************************************************/

/*
   NAME:  NETWORK SAMPLING ESTIMATION
   Author:  Steven K. Thompson
   Copyright (C) 2020 Steve Thompson, all rights reserved
*/
/* compile: */
/* time gcc -c -o libscratch.o libscratch.c -lm */
/* time gcc -Wall -o scratch scratch10.c libscratch.o -lm */
/* also can do in one step as */
/* time gcc -Wall -o scratch scratch10.c libscratch.c  */
/* for compilation on Windows check out Cygwin */
/* on Linux I compiled for Windows using */
/* x86_64-w64-mingw32-gcc -o scratch.exe scratch10.c libscratch.c */
/* it runs faster with optimazation and unrolling loops: */
/* time gcc -Wall  -Ofast -funroll-loops -o scratch scratch11.c libscratch.c -lm */



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <math.h>
//#include <tgmath.h>
#include <Windows.h>	
#include "scratch.h"  /* header files to go with scratch */
#include "stdafx.h"
#include "NetworkSampleEstimate.h"

 /* write output to a file, outputfile = 1, or terminal, outputfile = 0. */
int outputfile = 1;
long long int randomcalls = 0;  /* used in random number generation (mts) */
const char *buffer;
long length;

int NetworkSampleEstimatesConsoleOutput(int *tsteps_value, double *paad0_value, int *naad0_value, double *paadtrace_value,
	int *aadreseeddesign_value, double *paadrandom_value, int *naadtarget_value, int *coupons_value,
	char **nodeinputfilepath, char **nodefiinputfilepath, char **edgeinputfilepath, char **outputbuffer)
{
	char *tempOutput[] = { (char*)"c:\\data\\output.txt" };
	NetworkSampleEstimates(tsteps_value, paad0_value, naad0_value, paadtrace_value,
		aadreseeddesign_value, paadrandom_value, naadtarget_value, coupons_value,
		tempOutput, nodeinputfilepath, nodefiinputfilepath, edgeinputfilepath);

	FILE *tempOutPut = fopen(*tempOutput, "rb");

	if (tempOutPut)
	{
		fseek(tempOutPut, 0, SEEK_END);
		length = ftell(tempOutPut);
		fseek(tempOutPut, 0, SEEK_SET);
		buffer = malloc(length);
		if (buffer)
		{
			fread(buffer, 1, length, tempOutPut);
		}
		fclose(tempOutPut);
	}
	*outputbuffer = (char*) buffer;
	return 0;
}
								
/*
tsteps default 10000
paad0 default 0.0167
naad0 default 1
paadtrace default 0.5
aadreseeddesign default 1 
paadrandom default 0.01
naadtarget default 400
coupons default 0
*/
int NetworkSampleEstimates(int *tsteps_value, double *paad0_value, int *naad0_value, double *paadtrace_value,
			  int *aadreseeddesign_value, double *paadrandom_value, int *naadtarget_value, int *coupons_value,
			  char **outputfilepath, char **nodeinputfilepath, char **nodefiinputfilepath, char **edgeinputfilepath)
{
  extern NODE first;
  /* int dataid, databug, datadegree, dataconcurrent; */
  int i, count, count2;
  FILE *layoutp;
  struct node *tmp = NULL;
  struct edgenode *edgetmp = NULL;
  time_t time0, time1;
  clock_t clock0, clock1;
  time0 = time(NULL);
  clock0 = clock();
  int tsteps = *tsteps_value;  /* number of iterations of sampling process */
  
  /* select about paad0 * n initial units for the fast sampling process */
  double paad0 = *paad0_value; //0.0167;
  int naad0 = *naad0_value; //1;
  double paadtrace = *paadtrace_value; //0.5;
  int aadreseeddesign = *aadreseeddesign_value;// = 1;
  double paadrandom = *paadrandom_value; // 0.01;
  int naadtarget = *naadtarget_value;// 400;
  int coupons = *coupons_value;// 0;

  FILE *out_stream=NULL;
  if (outputfile == 1) /* change standard output to a file */
  {
	  if ((out_stream = freopen(*outputfilepath /*"c:\\data\\output.txt"*/, "w", stdout)) == NULL)
	  {
		  printf("Cannot open file: %s\n", *outputfilepath);
		  exit(1);
	  }
  }

  printf("\n");
  printf("********************************************************\n");
  printf("************  Network Sampling Estimation   ************\n");
  printf("********************************************************\n");
  /* printf("\n"); */

  /* printf("********************************************************\n");  */
  /* printf(" Thompson, S. (2020). Estimation for network snowball sampling:\n"); */
  /* printf("Preventing pandemics. arXiv:2009.04464.\n"); */
  printf("(Reference:  https://arxiv.org/abs/2009.04464) \n");
  /* printf("********************************************************\n"); */

  printf("\n");

  /**** Read node IDs and values from file ****/

  layoutp = fopen(*nodeinputfilepath /*"c:\\data\\nodes.txt"*/, "r");
  if (layoutp == NULL) {
    fprintf(stderr, "Can't open file %s\n", *nodeinputfilepath);
    exit(1);
  }

  char firstline[2048];

  fgets(firstline, sizeof(firstline), layoutp);
  /* printf("Node firstline %s sizeof(firstline) %ld\n", firstline, sizeof(firstline)); */

  size_t ln = strlen(firstline) - 1;
  if (*firstline && firstline[ln] == '\n') /* next line character */
    {
      firstline[ln] = '\0'; /* NULL character */
    }
  /* printf("ln strlen firstline %ld\n", ln);  */

  /* read column names from first line of node file */

  int ncolumns;
  /* char *namearray[100]; */
  char namearray[100][20];
  char *token;
  int nameColumns = 0;

  /* get first variable name */
  i=0;
  token = strtok(firstline," ");  /* first token, or variable name */
  /* get rest of variable names */
  while(token !=NULL)
    {
      strcpy(namearray[i], token); /* copy the token to the array */
      i = i +1;
      token = strtok(NULL," "); /* increments i before calculation */
    }

  nameColumns = i;
  int ndatavar;
  ndatavar = nameColumns - 1;

  printf("Node data %d columns with names:\n", nameColumns);
	  for(i=0;i< nameColumns;i++)
    {
      printf(" %s", namearray[i]);
    }
  printf("\n");

  count = 0;
  int datavalue;
  while (fscanf(layoutp, "%d", &datavalue) != EOF)
    {
      insert_front(datavalue);
      first->id = datavalue;
      if(count < 15)
	{
	  printf(" %d", first->id);
	}
      /* printf("first id %d count %d\n", datavalue, count); */
      first->datavar = (int *) malloc(sizeof(int) * ndatavar);
      for (i=0; i < ndatavar; i++)
	{
	  fscanf (layoutp, "%d", &datavalue);
	  /* printf("i %d datavalue %d\n", i, datavalue); */
	  /* insert_front(datavalue); */
	  first->datavar[i] = datavalue;
	}
      if (count < 15)
	{
	  for (i=0;i<ndatavar;i++)
	    {
	      printf(" %d", first->datavar[i]);
	    }
	  printf("\n");
	}
      first->bug = first->datavar[0];
      first->degree = first->datavar[1];
            count = count + 1;
    }

  int Nt = count;




  /*********************************************************/
  /****Read link IDs from file ****/
  /*********************************************************/

  int dataid1, dataid2;
      FILE *linksp;

      linksp = fopen(*edgeinputfilepath /*"c:\\data\\edges.txt"*/, "r");


      if (linksp == NULL) {
	fprintf(stderr, "Can't open file %s\n", *edgeinputfilepath);
	exit(1);
      }

   /* char linkfirstline[2048]; */

      /* read first line of link file for variable names */
fgets(firstline, sizeof(firstline), linksp);
   printf("Link firstline %s sizeof(firstline) %ld\n", firstline, sizeof(firstline));


  ln = strlen(firstline) - 1;
  if (*firstline && firstline[ln] == '\n')  /* newline charactter */
    {
      firstline[ln] = '\0';  /* NULL character */
    }
    /* printf("link ln strlen firstline %ld\n", ln); */

  /* read column names from first line of link file */
  /* up to 100 words of maximum length 20 */
  /* char *linknamesarray[100]; */
  char linknamesarray[100][20];



  /* get first variable name */
 /* get first variable name */
  i=0;
  token = strtok(firstline," ");  /* first token, or variable name */
  /* get rest of variable names */
  while(token !=NULL)
    {
      strcpy(linknamesarray[i], token); /* copy the token to the array */
      i = i +1;
      token = strtok(NULL," "); /* increments i before calculation */
    }



 ncolumns = i;
printf("Edge data %d columns with names:\n", ncolumns);
 for (i=0; i<ncolumns;i++)
   {
     printf(" %s", linknamesarray[i]);
   }
 printf("\n");



 count2 = 0;
 while(fscanf(linksp, "%d  %d\n", &dataid1, &dataid2) == 2)
   {
     /* skip loop edges */
     if (dataid1 != dataid2)
       {
	 insert_edge(dataid1,dataid2);
	 insert_edge(dataid2,dataid1);

       }
     if (count2 < 10)
       {
	 printf("%d  %d\n", dataid1, dataid2);
       }
     	 count2 = count2 + 1;
   }



 int nlinkvar;
 int linkestimates = 0;
 if (linkestimates == 0)
   {
	  nlinkvar = 0;
	}
      else  /* this calculates link variables edge degree, */
            /* link with edge degree > 0, hiv discrepant relationship */
	{
	  nlinkvar = 3;
      	  /* set edgetmp edgedegree */
	  printf("nlinkvar %d\n", nlinkvar);
	  count2 = 0;
	  tmp = first;
	  while (tmp != NULL)
	    {
	      edgetmp = tmp->adjfirst;
	      while (edgetmp != NULL)
		{
		  count2 = count2 + 1;
		  /* if (count2 < 10) */
		  /*   { */
		  /*     printf("count2 %d\n", count2); */
		  /*   } */
		  edgetmp->linkvar = (int *) malloc(sizeof(int) * nlinkvar);
		  for (i=0; i < nlinkvar; i++)
		    {
		      edgetmp->linkvar[i] = 0;
		    }
		  edgetmp->edgedegree = tmp->degree + edgetmp->idaddress->degree - 2;
		  edgetmp->linkvar[0] = edgetmp->edgedegree;
		  if (edgetmp->edgedegree > 0)
		    {
		      edgetmp->linkvar[1] = 1;
		    }
		  else
		    {
		      edgetmp->linkvar[1] = 0;
		    }
		  if(tmp->bug == edgetmp->idaddress->bug)
		    {
		      edgetmp->linkvar[2] = 0;
		    }
		  else
		    {
		      edgetmp->linkvar[2] = 1; /* discordant relationship */
		    }
		  edgetmp->fij = 0;
		  /* if (count2 < 10) */
		  /*   { */
		  /*     printf("link count %d\n", count); */
		  /*     printf("id1 %d id2 %d linkvar[0] %d, linkvar[1] %d\n", tmp->id, edgetmp->id, edgetmp->linkvar[0], edgetmp->linkvar[1]); */
		  /*   } */
		  edgetmp = edgetmp->adj;
		}
	      tmp = tmp->next;
	    }
   } /* if 0 */


      int nsampleedge = count2;
      printf("Number of nodes %d\n", Nt);
      printf("Number of edges: %d directed, %d undirected.\n", nsampleedge/2, nsampleedge);


      printf("\n");
      printf("********************************************************\n");
      printf("* Sampling process to estimate inclusion probabilities *\n");
      printf("********************************************************\n");
      /* printf("\n"); */

      /* random number generator stuff */
      struct mtstate *mts3 = NULL;
       unsigned long  sseed3 = 1;

       int seedsource = -1;
       /* get starting seed */
  if (seedsource == -1) /* seed from time in seconds */
    {
      sseed3 = time(NULL);
    }
  else if (seedsource == -2) /* hardware device random seeds */
    {
      sseed3 = (unsigned long) devrand();
    }

      /* give seed to random number generator (mersenne twister) */
      mts3 = createmtstate();
      init_genrand(mts3, sseed3);/* initialize random number stream mts3
				    for idealized designs  */

      	  /* bernoulli initial sample */
      count = 0;
	  tmp = first;
	  while ( tmp != NULL )
	    {
		  if (runif(mts3) < paad0)
		    {
		      tmp->aad[0]  = 1;
		      /* printf("id of initial fast sample unit %d\n", tmp->id); */
		      count = count + 1;
		    }
		  else
		    {
		      tmp->aad[0]  = 0;
		    }
	      tmp = tmp->next;
	    }
	  naad0 = count;
	  /* printf("initial fast sample size %d\n", naad0); */

	  /* end of fast sampling process */



/*******************************************************************/
/* Select tsteps fast samples to calculate inclusion frequencies f_i  */
/*******************************************************************/
	  int aadoutdegree = 0;
	  int  aadtotaldegree = 0;
	  int  maxselects = 0;
	  int maxfi = 1;
	  int naad = 0;
	  naad = naad0;
	  int naadlags = 0;
	  int aadreplace = 1;
	  int tempreplace = 0;
	  int aadfront = 0;
	  int aadback = 1;
	  double paadremove = 0.0;
	  int naad2 = 0;
	  int aadar = 2;
	  double aadarphi = 0.99;
	  double u;
	  double aadreseedprob = 0.0; /* check for different config value */


	  int t = 0;

 printf(" Selecting %d network samples \n", tsteps);
 printf("\n");
for (t = 1; t <= tsteps; t++)
  {

    	      /*** bernoulli tracing with adaptive acquisition ****
	       *** and attrition processes ************************
	       ****************************************************/



		  /* save old values add up outdegree from sample */
		  aadoutdegree = 0;
		  aadtotaldegree = 0;
		  maxselects = 0.0;
		  maxfi = 1;
		  count = 0;
		  naad = 0;
		  tmp = first;
		  while(tmp != NULL)
		    {
		      /* save old value of sample membership */
		      /* here, aad[0] should always be the current
			 value, whle aad[1], aad[2], ... aad[aadlags]
			 are that many lags in the past */
		      /* even if naadlags is 0, i still need to save
			 yesterday's value as aad[1] to prevent order-dependent
			 explosions within a time step */
		      if (naadlags == 0)
			{
			  tmp->aad[1] = tmp->aad[0];
			  edgetmp = tmp->adjfirst;
			}
		      else
			{
			  for (i = naadlags; i > 0;i--)
			    {
			      tmp->aad[i] = tmp->aad[i-1];
			    }
			}
		      /* add up active sample out degree if needed */
		      if (1)
			{
			  if (tmp->aad[0] == 1)
			    {
			      naad = naad + 1;
			      edgetmp = tmp->adjfirst;
			      while ( edgetmp != NULL )
				{
				  aadtotaldegree = aadtotaldegree + 1;
				  if (edgetmp->idaddress->aad[0] == 0)
				    {
				      aadoutdegree = aadoutdegree + 1;
				    }
				  if (aadreplace == 1 && edgetmp->idaddress->aad[0] == 2)
				    {
				      aadoutdegree = aadoutdegree + 1;
				    }
				  edgetmp = edgetmp->adj;
				}
			    }
			}
		      tmp = tmp->next;
		    }
		  /* printf("t %d in FAST 10 naad %d aadoutdegree %d\n",t, naad, aadoutdegree); */
		  /* printf("conductance %f out deg %d total deg %d\n", (float) aadoutdegree/aadtotaldegree, aadoutdegree, aadtotaldegree); */

		  /* temporarily sample with replacement if there are no other links to follow */
		  tempreplace = aadreplace;
		  if (aadreplace == 0 && aadoutdegree == 0 && aadfront == 1)
		    {
		      tempreplace = 1;
		    }
		  /* printf("  outdegree %d\n", aadoutdegree); */

		  /*** setting up tracing rate ***/
		  /* link tracing */
		  /* printf("     1 naad %d\n", naad); */
		  /* only trace if constant rate or sample size < target */
		  if (aadfront == 0 || (aadfront == 1 && naadtarget - naad > 0))
		    {
		      /* set tracing rate */
		      if (aadfront == 0)
			/* constant tracing rate */
			{
			  u = paadtrace;
			}
		      else /* if aadfront == 1 */
			/* control rate to give expected target sample size */
			{
			  if (naadtarget - naad > 0)
			    {
			      if (tempreplace != aadreplace)
				{
				  u = 1.0/aadoutdegree;
				}
			      else
				{
				  u = ((float) naadtarget - naad)/aadoutdegree;
				  if (u > 1.0) u = 1.0;
				}
			    }
			  else u = 0.0;
			}

		      /* printf("IN FAST 10 aadfront %d u %f\n", aadfront, u); */

		      /*** tracing ***/
		      /* now trace using rate u */
		      /* printf("ntarget - n %d, outdeg %d, u %f\n", (naadtarget-naad), aadoutdegree, u); */
		      tmp = first;
		      while (tmp != NULL)
			{
			  /* use the past value aad[1] so it's not
			     recursively traced if it turns 1 at this time */
			  if (tmp->aad[1] == 1)
			    {
			      edgetmp = tmp->adjfirst;
			      while ( edgetmp != NULL )
				{
				  /* make option for findtraced either
				     here or in finding design */
				      if (edgetmp->idaddress->aad[0] == 0 || (edgetmp->idaddress->aad[0] == 2 && tempreplace == 1))
					{
					  if (coupons == 0)
					    {
					      /* if ((edgetmp->safersex == 0 && runif(mts3) < u) || (edgetmp->safersex > 0 && runif(mts3) < u * safersexeffect)) */
					      if (runif(mts3) < u)
						{
						  edgetmp->idaddress->aad[0] = 1;
						  edgetmp->tracing = t;
						  naad = naad + 1;
						  /* printf("ADDED TRACED NODE TO FAST SAMPLE\n"); */
						}
					    }
					  if (coupons > 0)
					    {
					      if (tmp->coupon > 0 && runif(mts3) < u)
						{
						  edgetmp->idaddress->aad[0] = 1;
						  edgetmp->tracing = t;
						  edgetmp->idaddress->coupon = coupons;
						  tmp->coupon = tmp->coupon - 1;
						  naad = naad + 1;
						  /* printf("check 2  u %f, coupon %d id %d edge id %d\n",u, tmp->coupon, tmp->id, edgetmp->id); */
						}
					    }
					} /* if edgetmp idaddress aad[0] etc */



				  edgetmp = edgetmp->adj;
				}
			    }
			  tmp = tmp->next;
			}
		      /* end of tracing loop */



		    } /* if aadfront = 0 or (1 and less than target) */
		  /* attrition from aad sample */
		  /* printf("     2 naad %d\n", naad); */
		  /* only do if constant attrition rate or sample size > target */
		  if (aadback == 0 || (aadback == 1 && naad - naadtarget > 0))
		    {
		      /* set attrition rate */
		      if (aadback == 0)
			/* constant attrition rate */
			u = paadremove;
		      else /* if aadback == 1 */
			/* attrition rate to give expected target sample size */
			{
			  if (naad - naadtarget > 0)
			    {
			      u = ((float) naad - naadtarget)/naad;
			    }
			  else u = 0.0;
			}
		      /* attrition */
		      tmp = first;
		      while(tmp != NULL)
			{
			  if (tmp->aad[0] == 1 && runif(mts3) < u)
			    {
			      tmp->aad[0] = 2;
			      /* i may need to make transmission -30 here */
			      /* or use aad[0] to not delete on find step */
			      naad = naad - 1;
			      naad2 = naad2 + 1;
			      tmp->coupon = 0; /* no coupons out of sample */
			    }
			  tmp = tmp->next;
			}
		    } /* if aadback == 0 etc */
		  /* printf("     3 naad %d\n", naad); */


		  /* random selections, reseeds */
		  tmp = first;
		  while(tmp != NULL)
		    {
		      /*  random re-seeds */
		      if (aadreseeddesign == 1)
			{
			  aadreseedprob = paadrandom;
			} /* random reseeds */
		      /* reseeds with probability proportional to degree */
		      else if (aadreseeddesign == 2)
			{
			  /* want average of pp degree to be pfindingreseed */
			  /* c * sum_nodes tmp->degree/N_t = pfindreseed */
			  /* c * 2.0 * nedges/Nt = pfindreseed */
			  /* c = 0.5 * pfindreseed * Nt/nedges */
			  /* please check if Nt shoulb be nfound with separate */
			  /* sample structure */
			  aadreseedprob = 0.5 * paadrandom * Nt * tmp->degree/nsampleedge;
			}
		      /* select reseeds from nodes not in current sample */
		      if (tmp->aad[0] != 1)
			{
			  /* the factor 10.0 because this design needs higher
			     reseed prob than a without replacement one */
			  if (runif(mts3) < 1.0 * aadreseedprob)
			    {
			      tmp->aad[0] = 1;
			      /* printf("ADDED RANDOM NODE TO FAST SAMPLE\n"); */
			      /* tmp->aadselects[0] = tmp->aadselects[0] + 1; */
			      naad = naad + 1;
			      if (coupons > 0)
				{
				  tmp->coupon = coupons;
				}
			      /* if (coupons > tmp->degree) */
			      /* 	{ */
			      /* 	  tmp->coupon = tmp->degree; */
			      /* 	} */
			      /* else */
			      /* 	{ */
			      /* 	  tmp->coupon = coupons; */
			      /* 	} */
			    }
			}
		      tmp = tmp->next;
		    }

		  /* averaging aad values */
		  tmp = first;
		  while(tmp != NULL)
		    {
		      /* moving average over nlags + 1 steps */


		      if (aadar == 0)
			{
			  /* average aad over naadlags + 1 steps */
			  tmp -> aadlagav = 0.0;
			  for (i=0; i <= naadlags; i++)
			    {
			      /* printf("%d ", tmp->aad[i]); */
			      if (tmp->aad[i] == 1)
				{
				  tmp->aadlagav = tmp->aadlagav + 1.0;
				}
			    }
			  tmp->aadlagav = tmp->aadlagav/(1.0 + naadlags);
			}  /* if aadar == 0 */
		      else if (aadar == 1)
			/* autoregress */
			{
			  /* tmp->aadlagav = tmp->aadlagav/(1.0 - aadarphi); */
			  if (tmp->aad[0] == 1)
			    {
			      tmp->aadlagav = tmp->aadlagav * aadarphi + (1.0 - aadarphi) * tmp->aad[0];
			    }
			  else /* aad[0] = 0 or 2 both treated as 0 */
			    {
			      tmp->aadlagav = tmp->aadlagav * aadarphi;
			    }
			  /* tmp->aadlagav = tmp->aadlagav * (1.0 - aadarphi); */
			}
		      /* cumulative mean of aad */
		      else if (aadar == 2)
			/* autoregress */
			{
			  /* tmp->aadlagav = tmp->aadlagav/(1.0 - aadarphi); */
			  if (tmp->aad[0] == 1)
			    {
			      tmp->aadlagav = (-1.0 + t)/t * tmp->aadlagav + 1.0/t * tmp->aad[0];

			      /* make aadselects[0] 1 if aad[0] is 1 */
			      /* for use elsewhere in simple estimation */
			      /* tmp->aadselects[0] = 1; */
			      /* tmp->fi adds the raw frequency of inclusion */
			      /*  in the iterations  or ntsteps */

			      /* fi is the integer frequency of inclusion of */
			      /* a node in the fast sample */
			      /* fij is the frequency of indlusion of a link */
			      /* in the fast sample */
			      tmp->fi = tmp->fi + 1;
			      edgetmp = tmp->adjfirst;
			      while(edgetmp != NULL)
				{
				  /* if (edgetmp->findtraced == 1 ) */
				  /*   { */
				  if (edgetmp->idaddress->aad[0] == 1)
				    {
				      edgetmp->fij = edgetmp->fij + 1;
				    }
				  /* } */
				  edgetmp = edgetmp->adj;
				}
			    }
			  else /* aad[0] = 0 or 2 both treated as 0 */
			    {
			      tmp->aadlagav = tmp->aadlagav * (-1.0 + t)/t;
			      /* tmp->aadselects[0] = 0; */
			    }
			  /* tmp->aadlagav = tmp->aadlagav * (1.0 - aadarphi); */
			}
		      if (tmp->aadlagav > maxselects)
			{
			  /* printf("aadlagav %f maxselects %f id %d born %d\n", tmp->aadlagav, maxselects, tmp->id, tmp->eventtime[0]); */
			  maxselects = tmp->aadlagav;
			}
		      if (tmp->fi > maxfi)
			{
			  maxfi = tmp->fi;
			}

		      tmp = tmp->next;
		    } /* done averaging aad values */

  } /* t = 1 to tsteps */


/* done with fast design and calculating the fi valuies */

 /*********************************************************************/
 /*** Print the f_i to file nodesfi.txt *******************************/

 /*********************************************************************/

 /* declare the files */
 FILE *nodesfi_stream=NULL;
 /* FILe *edgesfi_stream=NULL; */

 /* give names to the files to write to */
 //char nodesfi_filename[] = "c:\\data\\nodesfi.txt";
 /* char edges_filename[] = "edgesfij.txt"; */

 /* open the files to write to */
 nodesfi_stream = fopen(*nodefiinputfilepath, "w");
 if (nodesfi_stream == NULL)
   {
     printf("can't file %s\n", *nodefiinputfilepath);
     exit(1);
   }
 /* edges_stream = fopen(edgesfij_filename, "w"); */
 /* if (edges_stream == NULL) */
 /*   { */
 /*     printf("can't open edgesfij file\n"); */
 /*     exit(1); */
 /*   } */


 /* write file headings */
 //fprintf(nodesfi_stream, "id bug degree concurrent fi\n");
 for (i = 0; i < nameColumns; i++)
 {
	 fprintf(nodesfi_stream, "%s ", namearray[i]);
 }
 fprintf(nodesfi_stream, "\n");
 /* fprintf(edgesfij_stream, "id1 id2 edgedegree fij\n"); */

 /* write node and edge data including the fi and fij to files */

 tmp = first;
 while (tmp != NULL)
   {

     fprintf(nodesfi_stream, "%d %d %d %d %d\n", tmp->id,  tmp->datavar[0], tmp->datavar[1], tmp->datavar[2], tmp->fi);

     /* fprintf(edges_stream, "%d %d %d %d\n", tmp->id, edgetmp->id,  edgetmp->edgedegree,  edgetmp->fij);			      */
     tmp = tmp->next;
   }

 /**************************************************************************/
 /* Calculate estimates and confidence intervals ***************************/
 /**************************************************************************/

 int j = 0;
   double * esttop = NULL;  /* top in simple estimator, then used for estimate */
  double * estbot = NULL;  /* bottom term in simple estimator */
  double * esttopf = NULL;  /* top in simple estimator, then used for estimate, based on f_i instead of aadlagav */
  double * estbotf = NULL;  /* bottom term in simple estimator, f_i based */

  double * estvarianceest = NULL; /* estimate of variance of estimate j */

    /* link degree or other edge variables */
  /* link simple estimator */
  double * linkesttop = NULL;  /* top in simple estimator, then used for estimate */
  double * linkesttopf = NULL;  /* top in link  estimator, stays top for var hat*/
  double * linkestbot = NULL;  /* bottom term in simple estimator */
   double * linkestvarianceest = NULL; /* estimate of variance of estimate j */

   double lower, upper;

   /* initialize estimation variables */

   esttop = (double *) malloc(sizeof(double) * ndatavar);
   estbot = (double *) malloc(sizeof(double) * ndatavar);
   esttopf = (double *) malloc(sizeof(double) * ndatavar);
   estbotf = (double *) malloc(sizeof(double) * ndatavar);
   estvarianceest = (double *) malloc(sizeof(double) * ndatavar);

   linkesttop = (double *) malloc(sizeof(double) * nlinkvar);
   linkesttopf = (double *) malloc(sizeof(double) * nlinkvar);
   linkestbot = (double *) malloc(sizeof(double) * nlinkvar);
   linkestvarianceest = (double *) malloc(sizeof(double) * nlinkvar);



 if (ndatavar > 0)
		{
		  for (j=0; j< ndatavar ; j++)
		    {
		      esttop[j] = 0.0;
		      estbot[j] = 0.0;
		      esttopf[j] = 0.0;
		      estbotf[j] = 0.0;
		      estvarianceest[j] = 0.0; /* estimate of variance of estimate j */
		    }
		}
	      if (nlinkvar > 0)
		{
		  for (j=0; j< nlinkvar ; j++)
		    {
		      linkesttop[j] = 0.0;
		      linkesttopf[j] = 0.0;
		      linkestbot[j] = 0.0;
		      linkestvarianceest[j] = 0.0;
		    }
		}



	      tmp = first;
	      while (tmp != NULL)
		{
		  /* here do the ndatavar estimates */
		  if (ndatavar > 0)
		    {

		      for (j=0; j< ndatavar ; j++)
			{
			  /* if (tmp->aadlagav/maxselects > 0.0000000001) */
			  if (tmp->fi > 0)
			    {
			      esttop[j] = esttop[j] + tmp->datavar[j]/tmp->aadlagav;
			      estbot[j] = estbot[j] + 1.0/tmp->aadlagav;
			      esttopf[j] = esttopf[j] + 1.0 * tmp->datavar[j]/tmp->fi;
			      estbotf[j] = estbotf[j] + 1.0/tmp->fi;
			    }
			} /* for j = 0; j<ndatavar ... */
		    } /* if ndatavar > 0 */

		  tmp = tmp->next;
		}


		  if (ndatavar > 0)
		    {
		      for (j=0; j< ndatavar ; j++)
			{
			  if (estbot[j] > 0.0000000001)
			    {
			      esttop[j] = esttopf[j]/estbotf[j]; /* changed to use fi */
			      /* esttopf[j] stays as top part for alt variance est */
			    }
			}
		    }

		  tmp = first;
		  while (tmp != NULL)
		    {
		      /* here do the ndatavar variance estimates */
		      if (ndatavar > 0)
			{
			  for (j=0; j< ndatavar ; j++)
			    {
			      estvarianceest[j] = estvarianceest[j] + 1.0/(Nt *(1.0 * Nt - 1.0)) * (1.0 * Nt * tmp->datavar[j]/tmp->fi -  esttopf[j]) * (1.0 * Nt * tmp->datavar[j]/tmp->fi -  esttopf[j]);
			    } /* for j = 0; j<ndatavar ... */
			} /* if ndatavar > 0 */
		      tmp = tmp->next;
		    }

		  /*****************************************************/
		  /********** link variable estimation *************/


		  		  if (nlinkvar > 0)
		    {
		      count2 = 0; /* count edges in sample */
		      tmp = first;
		      while (tmp != NULL)
			{
			  edgetmp = tmp->adjfirst;
			  while(edgetmp != NULL)
			    {
			      count2 = count2 + 1;
			      if (edgetmp->fij > 0)
				{
				  for (j=0; j< nlinkvar ; j++)
				    {

				      linkesttopf[j] = linkesttopf[j] + 1.0 * edgetmp->linkvar[j]/edgetmp->fij;
				      linkestbot[j] = linkestbot[j] + 1.0/edgetmp->fij;
				    }
				}
			      edgetmp = edgetmp->adj;
			    }
			  tmp = tmp->next;
			}
		    } /* if nlinkvar > 0 */


		  if (nlinkvar > 0)
		    {
		      for (j=0; j < nlinkvar ; j++)
			{
			  linkesttop[j] = linkesttopf[j]/linkestbot[j];
			}
		    }

		  /* now the nlinkvar link variance estimates */
		  if (nlinkvar > 0)
		    {
		      tmp = first;
		      while (tmp != NULL)
			{
			  edgetmp = tmp->adjfirst;
			  while(edgetmp != NULL)
			    {

			      for (j=0; j< nlinkvar ; j++)
				{
				  linkestvarianceest[j] = linkestvarianceest[j] + 1.0/(nsampleedge *(1.0 * nsampleedge - 1.0)) * (1.0 * nsampleedge * edgetmp->linkvar[j]/edgetmp->fij -  linkesttopf[j]) * (1.0 * nsampleedge * edgetmp->linkvar[j]/edgetmp->fij -  linkesttopf[j]);
				}
			      edgetmp = edgetmp->adj;
			    }
			  tmp = tmp->next;
			}
		    }


		  printf("**************************************************\n");
		  printf("***     Estimation for mean of variable j      ***\n");
		  printf("**************************************************\n");

		  printf("j     variable  estimate  .95 confidence interval\n");

      for (j=0; j< ndatavar ; j++)
	{
	  lower = esttop[j] - 1.959964 * sqrt(estvarianceest[j]);
	  upper = esttop[j] + 1.959964 * sqrt(estvarianceest[j]);

	  printf("%d %12s  %f  (%f, %f)\n", j, namearray[j+1], esttop[j], lower, upper);

	}
      printf("**********************************************\n");

printf("\n");

      if (linkestimates) /* do or don't estimate link variables */
	{
	  /* confidence intervals for data variables */

	  printf("\n");

	  char linkdatanames[10][20] ={"edge degree", "edge deg > 0", "discordant"};
	  printf("confidence intervals for link variables\n");
	  printf("j         name  estimate    (lower, upper)\n");

      for (j=0; j< nlinkvar ; j++)
      	{
      	  lower = linkesttop[j] - 1.959964 * sqrt(linkestvarianceest[j]);
      	  upper = linkesttop[j] + 1.959964 * sqrt(linkestvarianceest[j]);

	  printf("%d %12s  %f  (%f, %f)\n", j, linkdatanames[j], linkesttop[j], lower, upper);
      	}



      printf("**********************************************\n");

	} /* if (0) */


      printf("**********************************************\n");


      /* calculate computing run time */
  time1 = time(NULL);
  clock1 = clock();
  printf("Wall time %ld seconds \n", (long) (time1-time0));
  printf("CPU time %f seconds \n", (float) (clock1-clock0)/CLOCKS_PER_SEC);
  printf("\n");
  if (outputfile == 1)
    {
      fclose(out_stream);
    }
  fclose(nodesfi_stream);
  fclose(layoutp);
  fclose(linksp);

   return 0;
} /* end of main */

