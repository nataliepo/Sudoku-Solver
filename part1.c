/**************************************
 * FILE: part1.c                      *
 * NAME: Natalie Podrazik             *
 * DATE: 11 December 2005             *
 * CLASS: CS 441 - Algortihms - White *
 *                                    *
 *  This file contains the main driver*
 * for a program that finds a solution*
 * (if possible) given a certain file *
 * of input.  It tries to rule out the*
 * possibilities for a particular     *
 * Sudoku cell's pip values, but if   *
 * more searching must be done, it    *
 * exits to the outer PERL program to *
 * notify of this condition.  If no   *
 * solution can be found, it exits on *
 * another escape code to notify the  *
 * outer program of this condition.   *
 * Otherwise, if it finds a solution, *
 * it prints it to the output file.   *
 **************************************/ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "util.h"

int main(int argc, char *argv[])
{
   FILE * ifp;  
   int *a;     
   int i, j;
   TRACE *t;
   int row = 0; 
   int col = 0;
   int pip;
   int numInRow = 0;
   int temp;
   
   SIZE = 0;
   DIMEN = 0;
   REMAINING = 0;
   SUM = 0;
   NUMDIGITS = 0;

   if (argc < 3)
   {
      fprintf(stderr, "3 cmdline args needed: exe, input, output\n");
      exit(1);
   }

   ifp = fopen(argv[1], "r");
   if (ifp == NULL)
   {
      fprintf(stderr, "could not open file %s\n", argv[1]);
      exit(2);
   }

   /* scan in the whole file. */
   while (fscanf(ifp, "%d", &temp) != EOF)
   {
      numInRow++;
   }
   numInRow = sqrt(numInRow);  

   SIZE = sqrt(numInRow);
   DIMEN = numInRow;
   REMAINING = DIMEN * DIMEN;

   /* num digits is 1 to DIMEN */
   NUMDIGITS = DIMEN + 1;
   
   /* SUM is the sum of all possibilities */
   for (i = 0; i < NUMDIGITS; i++)
   {
      SUM =+ i;
   }

   /* allocate space and make sure you have some left */
   a = (int *) malloc(sizeof(int) * REMAINING);   
   t = (TRACE *)malloc(sizeof(TRACE) * REMAINING);   
   if ((a == NULL) ||
       (t == NULL))
   {
      fprintf(stderr, "OUT OF MEMORY, exiting.\n");
      exit(-1);
   }
   
   
   /* go back a row */
   rewind (ifp);   

   for (i = 0; i < DIMEN; i++)
   {
      for (j = 0; j < DIMEN; j++)
      {
         fscanf(ifp, "%d", AtI(a, i, j));
	 
	 /* allocate space for each bmp array. */
	 AtT(t, i, j)->bmp = (int*) malloc(sizeof(int)*NUMDIGITS);
         if (AtT(t, i, j)->bmp == NULL)
	 {
	    fprintf(stderr, "OUT OF MEMORY, exiting.\n");
	    exit(-2);
	 }
      }
   }

   fclose(ifp);
   
   /* initialize the TRACE array, then set up the pip values. */
   InitializeTrace(t);
   SetupTrace(a, t);         

   pip = 1;
   
   /* keep ruling out possibilities until you find the solution or *
    * get stuck */
   while (REMAINING > 0)
   {
      pip = FindObvious(t, &row, &col);
   
      /* if it finds the obvious choice, returns pip *
       * otherwise, it returns -1                    */
      if (pip > 0)
      {
         Found(t, row, col, pip);
      }
      else
      {	
         pip = FindRowAns(t, &row, &col);
         if (pip > 0)
         {	    	    
            Found(t, row, col, pip);	   
         }
         else
         {           
            pip = FindColAns(t, &row, &col);
            if (pip > 0)
            {
               Found(t, row, col, pip);                             
            }
            else
            {
               pip = FindNeighborhoodAns(t, &row, &col); 
               if (pip > 0)
               {
                  Found(t, row, col, pip);                 
               }
               else
               {  
		  /* If it gets here, this input could still be *
		   * valid, but it needs to be searched on.     */
                  DumpTrace(t);          
		  CleanUp(t, a);		
                  exit(1);
               }	      
	    }
	 } 
      }
   }  

   /* If it gets here, a solution was found. yey! */
   PrintSolution(argv[2], t);   
   
   /* free memory */
   CleanUp(t, a);
   
   return 0;
}

