/**************************************
 * FILE: util.c                       *
 * NAME: Natalie Podrazik             *
 * DATE: 11 December 2005             *
 * CLASS: CS 441 - Algortihms - White *
 *                                    *
 *  This file contains the function   *
 * definitions used in the simple     *
 * Sudoku.exe program that does not   *
 * search for a solution, but simply  *
 * determines if a solution can be    *
 * determined from the input.         *
 **************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "util.h"


/*********************************************
 * CleanUp()                                 *
 *  Input:                                   *
 *    t - array of TRACE cells               *
 *    a - array of temporary integers (input)*
 * Frees the memory allocated by t and a.    *
 *********************************************/
void CleanUp(TRACE * t, int * a)
{
   int i, j;

   for (i = 0; i < DIMEN; i++)
   {
      for (j = 0; j < DIMEN; j++)
      {
	 free (AtT(t, i, j)->bmp);
      }
   }
   
   free(t);
   free(a);
}


/*********************************************
 * FindObvious()                             *
 *  Input:                                   *
 *    t - array of TRACE cells               *
 *    row - pointer to a row variable        *
 *    col - pointer to a col varialbe        *
 * Finds an "obvious" solution by searching  *
 * through t for a cell with all other       *
 * solutions ruled out.  Returns the pip     *
 * value found on success and changes the row*
 * and col variables to hold the index of the*
 * cell; returns -1 if there are no remaining*
 * "obvious" solutions.                      *
 *********************************************/
int FindObvious(TRACE *t, int * row,  int * col)
{
   int i, j, k;

   for (i = 0; i < DIMEN; i++)
   {
      for (j = 0; j < DIMEN; j++)
      {	         
	 if ((AtT(t, i, j)->numPoss == 1) &&
	     (AtT(t, i, j)->sol <= 0))
         {
            *row = i;
            *col = j;
            k = 1;
	              
	    while ((k < NUMDIGITS) &&
		   (AtT(t, i, j)->bmp[k] != UNKNOWN))
	      
            {
               k++;
            }
            return k; 
         }
      }
   }

   *row = -1;
   *col = -1;
   return -1;
}


/*********************************************
 * FindNeighborhoodAns()                     *
 *  Input:                                   *
 *    t - array of TRACE cells               *
 *    row - pointer to a row variable        *
 *    col - pointer to a col varialbe        *
 * Finds a "neighborhood" solution by        *
 * through each neighborhood in t for each   *
 * pip value to see which ones still need to *
 * be solved.  If only one needs to be solved* 
 * then it can find a solution. Returns the  *
 * pip value found and saves the row and col *
 * into the parameter's pointers; otherwise  *
 * returns -1.                               *
 *********************************************/
int FindNeighborhoodAns(TRACE *t, int * row, int * col)
{
   int numUnknown;
   int i, j, k, l, m;
   int thisRow, thisCol;

   for (k = 1; k < NUMDIGITS; k++)
   {
      for (i = 0; i < DIMEN; i += SIZE)
      {
         for (j = 0; j < DIMEN; j += SIZE)
         {
            numUnknown = 0;
            for (l = i; l < i + SIZE; l++)
            {
               for (m = j; m < j + SIZE; m++)
               {		                   
		  if (AtT(t, l, m)->bmp[k] == UNKNOWN)
                  {
                     numUnknown++;
                     thisCol = m;
                     thisRow = l;
                  }
               }
            }	   
            if (numUnknown == 1)
            {
               *row = thisRow;
               *col = thisCol;
               return k;
            }
         }
      }
   }

   return -1;
}
               


/*********************************************
 * FindColAns()                              *
 *  Input:                                   *
 *    t - array of TRACE cells               *
 *    row - pointer to a row variable        *
 *    col - pointer to a col varialbe        *
 * Finds a "column" solution by              *
 * through each column       in t for each   *
 * pip value to see which ones still need to *
 * be solved.  If only one needs to be solved* 
 * then it can find a solution. Returns the  *
 * pip value found and saves the row and col *
 * into the parameter's pointers; otherwise  *
 * returns -1.                               *
 *********************************************/
int FindColAns(TRACE * t, int * row, int * col)
{
   int numUnknown;
   int i, j, k;
   int thisRow = 0, thisCol = 0;

   for (j = 0; j < DIMEN; j++)
   {
      thisCol = j;
      for (k = 1; k < NUMDIGITS; k++)
      {
         numUnknown = 0;
         for (i = 0; i < DIMEN; i++)
         {           
	    if (AtT(t, i, j)->bmp[k] == UNKNOWN)
            {
               numUnknown++;
               thisRow = i;
            }
         }
         if (numUnknown == 1)
         {
            *row = thisRow;
            *col = thisCol;
            return k;
         }
      }
   }

  return -1;
}


/*********************************************
 * FindRowAns()                              *
 *  Input:                                   *
 *    t - array of TRACE cells               *
 *    row - pointer to a row variable        *
 *    col - pointer to a col varialbe        *
 * Finds a "row"    solution by              *
 * through each row          in t for each   *
 * pip value to see which ones still need to *
 * be solved.  If only one needs to be solved* 
 * then it can find a solution. Returns the  *
 * pip value found and saves the row and col *
 * into the parameter's pointers; otherwise  *
 * returns -1.                               *
 *********************************************/
int FindRowAns(TRACE * t, int * row, int * col)
{
   int numUnknown;
   int i, j, k;
   int thisRow = 0, thisCol = 0;   
   
   for (i = 0; i < DIMEN; i++)
   { 
      thisRow = i;
      
      for (k = 1; k < NUMDIGITS; k++)
      {
	 numUnknown = 0;
	 
	 for (j = 0; j < DIMEN; j++)
	 {          
	    if (AtT(t, i, j)->bmp[k] == UNKNOWN)
	    {
	       numUnknown++;
	       thisCol = j;
	    }       
	 }
	 
	 if (numUnknown == 1)
	 {
	    *row = thisRow;
	    *col = thisCol; 
	    return k;
	 }
      }
   }
   return -1;
}


/*********************************************
 * SetupTrace()                              *
 *  Input:                                   *
 *    t - array of TRACE cells               *
 *    a - array of integers from input file  *
 * Initializes the array of TRACE cells by   *
 * assigning solutions based on the contents *
 * of a.  If a cell in a contains a digit    *
 * other than 0, then call Found() on that   *
 * cell to rule out the possibilities of the *
 * surrounding cells to have that pip.       *
 *********************************************/
void SetupTrace(int * a, TRACE * t)
{
   /* going to put all the junk that's needed into the TRACE array */
   int i, j;
   int digit = 0;
 
   for (i = 0; i < DIMEN; i++)
   {
      for (j = 0; j < DIMEN; j++)
      {
	 digit = *(AtI(a, i, j));
	 
         /*if (a[i][j] != 0) */
	 if (digit != 0)
         {             
	    Found(t, i, j, digit);
         }
      }
   }      
}


/*********************************************
 * Found()                                   *
 *  Input:                                   *
 *    t - array of TRACE cells               *
 *    row - row index                        *
 *    col - column index                     *
 *    pip - pip value                        *
 * Sets the contents of t[row][col] to have  *
 * the solution pip.  Modifies the bmp array *
 * of t[row][col] to contain all N's, except *
 * for the Y on the solution.  Also rules out*
 * t[row][col]'s row, column, & neighborhood.*
 * Decrements the number of remaining        *
 * solutions.                                *
 *********************************************/
void Found(TRACE *t, int row, int col, int pip)
{   
   int k;
  
   AtT(t, row, col)->sol = pip;

   for (k = 1; k <= DIMEN; k++)
   {
      if (k != pip)
      {         
	 AtT(t, row, col)->bmp[k] = NO;
      }
   }

   AtT(t, row, col)->bmp[pip] = YES;
   AtT(t, row, col)->numPoss = 1;

   REMAINING--;
   
   RuleOutRow(t, row, col, pip);   
   RuleOutCol(t, row, col, pip);  
   RuleOutNeighborhood(t, row, col, pip);   
}

  
/*********************************************
 * RuleOutRow()                              *
 *  Input:                                   *
 *    t - array of TRACE cells               *
 *    row - row index                        *
 *    col - column index                     *
 *    pip - pip value                        *
 * Sets the pip value of every column in     *
 * this row to be a 'N', and decrements the  *
 * number of remaining solutions for that    *
 * cell.  If there are no remaining solutions*
 * then the puzzle cannot be solved, & exits.*
 *********************************************/
void RuleOutRow(TRACE * t, int row, int col, int pip)
{
   int j;

   for (j = 0; j < DIMEN; j++)
   {
      if (j != col)
      {	 
	 if (AtT(t, row, j)->bmp[pip] == UNKNOWN)
         {	   
	    AtT(t, row, j)->bmp[pip] = NO;
	    (AtT(t, row, j)->numPoss)--;
	    
            /* if there are no more possibilities, there is no soln. */
	    if (AtT(t, row, j)->numPoss == 0)
	    {
	       free(t);	
	       exit(2);
	    }
         }
      }
   }
}


/*********************************************
 * RuleOutCol()                              *
 *  Input:                                   *
 *    t - array of TRACE cells               *
 *    row - row index                        *
 *    col - column index                     *
 *    pip - pip value                        *
 * Sets the pip value of every row in this   *
 * column   to be a 'N', and decrements the  *
 * number of remaining solutions for that    *
 * cell.  If there are no remaining solutions*
 * then the puzzle cannot be solved, & exits.*
 *********************************************/
void RuleOutCol(TRACE * t, int row, int col, int pip)
{
   int i;

   for (i = 0; i < DIMEN; i++)
   {
      if (i != row)
      {
	 if (AtT(t, i, col)->bmp[pip] == UNKNOWN)
         {	    
	    AtT(t, i, col)->bmp[pip] = NO;
	    (AtT(t, i, col)->numPoss)--;
	    
            /* if there are no more possibilities, there is no soln. */
	    if (AtT(t, i, col)->numPoss == 0)
	    {
	       free(t);
	       exit(2);
	    }
         }
      }
   }
}


/*********************************************
 * RuleOutNeighborhood()                     *
 *  Input:                                   *
 *    t - array of TRACE cells               *
 *    row - row index                        *
 *    col - column index                     *
 *    pip - pip value                        *
 * Sets the pip value of every cell   in     *
 * this neighborhood to be a 'N', and also   *
 * decrements the number of remaining        *
 * solutions for that cell.  If there are no *
 * remaining solutions then the puzzle cannot*
 * be solved, & exits.                       *
 *********************************************/
void RuleOutNeighborhood(TRACE * t, int row, int col, int pip)
{
   int i, j;
   int startRow, startCol;

   /* take advantage of integer division and 0 indexing */
   startRow = row / SIZE;
   startCol = col / SIZE;

   /* now find coordinate where it should start */
   startRow *= SIZE;
   startCol *= SIZE;
 
   for (i = startRow; i < startRow + SIZE; i++)
   {
      for (j = startCol; j < startCol + SIZE; j++)
      {
         if (! ((i == row) && (j == col)))
         {	            
	    if (AtT(t, i, j)->bmp[pip] == UNKNOWN)
            {	       
	       AtT(t, i, j)->bmp[pip] = NO;
	       AtT(t, i, j)->numPoss--;
	       
	       /* if there are no more possibilities, there is no soln. */
	       if (AtT(t, i, j)->numPoss == 0)
	       {
		  free(t);
		  exit(2);
	       }		  
            }
         }
      }
   }
}


/*********************************************
 * InitializeTrace()                         *
 *  Input:                                   *
 *    t - array of TRACE cells               *
 * Sets the bmp array of every cell in t to  *
 * be '?' and every numPoss to be the max #  *
 * of pips (DIMEN).                          *
 *********************************************/
void InitializeTrace(TRACE * t)
{
   int i, j, k;

   for (i = 0; i < DIMEN; i++)
   {
      for (j = 0; j < DIMEN; j++)
      {
         for (k = 0; k <= DIMEN; k++)
         {	  
            AtT(t, i, j)->bmp[k] = UNKNOWN;
         }
	
	 AtT(t, i, j)->numPoss = DIMEN;
      }
   }
}


/*********************************************
 * PrintSolution()                           *
 *  Input:                                   *
 *    t - array of TRACE cells               *
 *    file - file to print to                *
 * Prints the solution of this puzzle out to *
 * the file.                                 *
 *********************************************/
void PrintSolution(char * file, TRACE *t)
{
   int i, j, k;
   FILE * ofp;
  
   ofp = fopen(file, "w");

   for (i = 0; i < DIMEN; i++)
   {
      if ((i > 0) && (i % SIZE != 0))
      {
	 for (j = 0; j < SIZE; j++)
	 {
	    fprintf(ofp, "|");
	    for (k = 0; k < SIZE; k++)
	    {
	       fprintf(ofp, "    ");
	    }
	 }
	 fprintf(ofp, "|\n");
      }
     
      else
      {
	 for (j = 0; j < SIZE; j++)
	 {
	    fprintf(ofp, "+");
	    for (k = 0; k < SIZE; k++)
	    {
	       fprintf(ofp, "----");
	    }
	 }
	 fprintf(ofp, "+\n");
      }

      for (j = 0; j < DIMEN; j++)
      {
         if (j % SIZE == 0)
         {
            fprintf(ofp, "|");
         }
   
	 if ((AtT(t, i, j)->sol < 0) || (AtT(t, i, j)->sol > 9))
         {	 
	    fprintf(ofp, " %d ", AtT(t, i, j)->sol);
         }
         else
         {           
	    fprintf(ofp, "  %d ", AtT(t, i, j)->sol);
         }
      }

      fprintf(ofp, "|\n");
   }

   for (j = 0; j < SIZE; j++)
   {
      fprintf(ofp, "+");
      for (k = 0; k < SIZE; k++)
      {
	 fprintf(ofp, "----");
      }
   }
   fprintf(ofp, "+\n");

   fclose(ofp);
}


/*********************************************
 * DumpTrace()                               *
 *  Input:                                   *
 *    t - array of TRACE cells               *
 * Dumps out the contents of the TRACE array *
 * to be read in for the Perl program for    *
 * searching.                                *
 *********************************************/
void DumpTrace(TRACE * t)
{
   int i, j, k;
   int numUnknown; 

   /* print the dimensions, save soem time. */
   printf("%d\n", DIMEN);

   /* now print out things that are ruled out */
   for (i = 0; i < DIMEN; i++)
   {
      for (j = 0; j < DIMEN; j++)
      {		 
	 numUnknown = 0;

         for (k = 1; k < NUMDIGITS; k++)
         {                        
	    switch (AtT(t, i, j)->bmp[k])
            {
              case UNKNOWN:
                 printf("?  ");
		 numUnknown++;
                 break;
              case NO:
                 printf("N  ");
                 break;
              case YES:
                 printf("Y  ");
                 break;
              default:
                 printf("!  ");
                 break;
            }
         }
	 
	 if (AtT(t, i, j)->sol > 0)
	 {
	    printf(" %d ", AtT(t, i, j)->sol);
	 }
         printf("\n");
      }  
   }
}


/*********************************************
 * AtT()                                     *
 *  Input:                                   *
 *    t - array of TRACE cells               *
 *    i - the row to index                   *
 *    j - the column to index                *
 * Returns a pointer to the cell at the      *
 * logical t[i][j] place.                    *
 *********************************************/
TRACE * AtT(TRACE * matrix, int i, int j)
{
  return (&matrix[(i * DIMEN) + j]);
}

/*********************************************
 * AtI()                                     *
 *  Input:                                   *
 *    matrix - array of integer cells        *
 *    i - the row to index                   *
 *    j - the column to index                *
 * Returns a pointer to the cell at the      *
 * logical matrix[i][j] place.               *
 *********************************************/
int * AtI(int * matrix, int i, int j)
{
   return (&matrix[(i * DIMEN) + j]);
}
