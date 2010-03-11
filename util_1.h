/**************************************
 * FILE: util.h                       *
 * NAME: Natalie Podrazik             *
 * DATE: 11 December 2005             *
 * CLASS: CS 441 - Algortihms - White *
 *                                    *
 *  This file contains the function   *
 * prototypes  used in the simple     *
 * Sudoku.exe program that does not   *
 * search for a solution, but simply  *
 * determines if a solution can be    *
 * determined from the input.         *
 **************************************/

#ifndef _UTIL_H_
#define _UTIL_H_

#define UNKNOWN -1
#define NO  0
#define YES 1
#define D 3
 

/* struct used for recording the data. */

typedef struct trace
{
   int * bmp;
   int numPoss;
   int sol;   /* the solution */
} TRACE;


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
void Found(TRACE* t, int row, int col, int pip);

/*********************************************
 * InitializeTrace()                         *
 *  Input:                                   *
 *    t - array of TRACE cells               *
 * Sets the bmp array of every cell in t to  *
 * be '?' and every numPoss to be the max #  *
 * of pips (DIMEN).                          *
 *********************************************/
void InitializeTrace(TRACE * t);

/*********************************************
 * PrintSolution()                           *
 *  Input:                                   *
 *    t - array of TRACE cells               *
 *    file - file to print to                *
 * Prints the solution of this puzzle out to *
 * the file.                                 *
 *********************************************/
void PrintSolution(char * file, TRACE * t);

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
void RuleOutRow(TRACE * t, int row, int col, int pip);

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
void RuleOutCol(TRACE * t, int row, int col, int pip);

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
void RuleOutNeighborhood(TRACE *t, int row, int col, int pip);

/*********************************************
 * DumpTrace()                               *
 *  Input:                                   *
 *    t - array of TRACE cells               *
 * Dumps out the contents of the TRACE array *
 * to be read in for the Perl program for    *
 * searching.                                *
 *********************************************/
void DumpTrace(TRACE *t);

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
void SetupTrace(int * a, TRACE *t);

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
int  FindObvious(TRACE *t, int * row, int * col);

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
int  FindRowAns (TRACE * t, int * row, int * col);

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
int  FindColAns (TRACE * t, int * row, int * col);

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
int  FindNeighborhoodAns(TRACE * t, int * row, int * col);
TRACE * AtT(TRACE * matrix, int i, int j);
int * AtI (int * matrix, int i, int j);

/*********************************************
 * CleanUp()                                 *
 *  Input:                                   *
 *    t - array of TRACE cells               *
 *    a - array of temporary integers (input)*
 * Frees the memory allocated by t and a.    *
 *********************************************/
void CleanUp(TRACE * t, int * a);


/* globals used for dimension and solution count */
int REMAINING;
int SIZE;
int DIMEN;
int SUM;
int NUMDIGITS;

#endif
