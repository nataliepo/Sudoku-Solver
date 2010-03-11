#!/usr/bin/perl -w

#----------------------------------
# FILE: proj.pl
# NAME: Natalie Podrazik
# DATE: 11 December 2005
# CLASS: CS 441 - Algorithms - White
#
#   This file uses the executable
# "Sudoku.exe" to find solutions to 
# Sudoku puzzles in Perl and C.
#----------------------------------

# check command line args.
if ($#ARGV < 1)
{
   die "Usage: <scriptname> <inputFile of SuDoku puzzle> <answer as outputfile> <optional DumpFilename>\n";
}

# in case they don't enter one
$dumpFile = "temp.out";

if ($#ARGV >= 2)
{
   $dumpFile = $ARGV[2];
}

$puzzleFile = $ARGV[0];
$answerFile = $ARGV[1];

$command = "./Sudoku.exe " . $puzzleFile . " " . $answerFile;

$command = $command . " > " . $dumpFile;
 
$result = system($command);

if ($result == 0)
{
    printf "\n\tSolution found! Printed to \"%s\"\n", $answerFile;
    die "\n" ;
}

# since there are still solutions to find, we shall find them!  

#if we can't open the dump file, or it's 0 size, the puzzle is impossible, and the
# user is trying to trick us.
if (-z $dumpFile) {
  die "\n\tThis puzzle is impossible to solve!\n\n";
}
open (IN, $dumpFile) or die "\n\tThis puzzle is impossible to solve!\n\n";

#read in the dimension
$dimen = <IN>;


while ($line = <IN>)
{
  $line =~ m/([YN? \d]+)/;
  
  $dump = $1;  

  push(@board, $dump);
  
}
close(IN);

$row = 0;
$col = 0;

#find the first spot in the array that doesnt have a solution
FindFirstUnsolved(\@board, $dimen);

#this returns the possibilities in an array called @poss of that spot
FindPoss(At(\@board, $row, $col, $dimen), $dimen);

# for every possibility, create a new board to search upon
for ($i = 0; $i < @poss; $i++) {
  $newArray = MakeNewArray(\@board, $dimen, $row, $col, $poss[$i]);
  push (@newBoards, $newArray);
}

$tempFile = "temp.dat";
$result = 1;

while ((@newBoards) && ($result != 0)) {

  #print "another board : ";
  $boardRef = pop(@newBoards);
  PrintBoardToFile ($boardRef, $tempFile, $dimen);

  $command = "Sudoku.exe " . $tempFile . " " . $answerFile;

  $command = $command . " > " . $dumpFile;
     
  
  $result = system($command);
 
  $result = sprintf("%d", $result);

  if ($result == 0) {
    printf "\n\tSolution found! Printed to \"%s\"\n", $answerFile;
    die "\n";
  }
  elsif ($result == 256) {

    # clear the temporary board array.
    @board = ();

    open (IN, $dumpFile);    
    $dimen = <IN>;       
    
    # store what you read into @board
    while ($line = <IN>) {

      $line =~ m/([YN? \d]+)/;
      $dump = $1;
      
    
      
      push(@board, $dump);      
    }
    close(IN);

    $row = 0;
    $col = 0;

    FindFirstUnsolved(\@board, $dimen);

    #this returns the possibilities in an array called @poss
    FindPoss(At(\@board, $row, $col, $dimen), $dimen);

    for ($i = 0; $i < @poss; $i++) {
      $newArray = MakeNewArray(\@board, $dimen, $row, $col, $poss[$i]);
      push (@newBoards, $newArray);
    }          
  } 
}

print "\n\tNo solution to this puzzle.\n";


#--------------------------------
# MakeNewArray()
#   Returns a reference to an 
#  array just created.
# Parameters:
#   1. address of old board
#   2. dimension
#   3. row to change
#   4. column to change
#   5. guess to change the old board
#--------------------------------
sub MakeNewArray {
  my $board = shift();
  my @board = @$board;
  my $dimen = shift();
  my $row = shift();
  my $col = shift();
  
  # this is what the pip at board[row][col] will be.
  my $guess = shift();
  
  #clear the string
  my $str = "";

  my @newArray;

  for (my $i = 0; $i < $dimen; $i++) {
    for (my $j = 0; $j < $dimen; $j++) {

      # this is the row to change
      if (($i == $row) && ($j == $col)) {
	$str = "";
	for (my $k = 1; $k <= $dimen; $k++) {
   
           # this pip is yes
           if ($k == $guess) {
             $str = $str . "Y  ";
           }
           else {
             $str = $str . "N  ";
           }     
	 } 
	$str = $str . "  " . $guess;
      }

      # don't change this row 
      else {
        $str = At($board, $i, $j, $dimen);
      }      
      push (@newArray, $str);
    }
  }

  return \@newArray;
}
   


#--------------------------------
# PrintBoardToFile()
#   Prints the board to an output
# file to be read into the SuDoKu.exe
# Parameters:
#   1. address of old board
#   2. filename to print to
#   3. dimension
#--------------------------------
sub PrintBoardToFile {
  my $board = shift();
  my @board = @$board;
  my $file = shift();
  my $dimen = shift();

  my $pip = 0;
  my $str = "";

  open (OUTFILE, "> " . $file) or die "Could not open file " . $file . "\n"; 
  

  for (my $i = 0; $i < $dimen; $i++) {    
    for (my $j = 0; $j < $dimen; $j++) {  
      
      # grab the string
      $str = At($board, $i, $j, $dimen);
      
      # if the pip is there, print it...otherwise, 0.
      if ($str =~ m/([\d])/) {
	$pip = $1;
      }
      else {
	$pip = 0;
      }
      print OUTFILE $pip . " ";      
    }
    print OUTFILE "\n";
  } 

  close(OUTFILE);      
}

#--------------------------------
# FindPoss()
#   Finds all the possibilities of pips
# for a particular string passed in.
# Parameters:
#   1. string (of "N  N  ? ...")
#   2. dimension
# Sets the array @poss to contain
#  what the possibilities are for
#  this string, and returns it.
#--------------------------------
sub FindPoss {
  my $dump = shift();
  my $dimen = shift(); 

  #split the string around the spaces that separate it.
  @items = split /  /, $dump;

  $size = @items;
  #clear hte possibilities array 
  @poss = ();
  for (my $i = 0; $i < $size; $i++) {
    if ($items[$i] =~ /\?/) {
       push (@poss, $i+1);
     }
  }

  return @poss;
}


#--------------------------------
# FindFirstUnsolved()
#   Finds the first box in the 
# array that contains the symbol '?'.
# Changes the $row and $col 
# global variables.
# Parameters:
#   1. address of the board
#   2. dimension
#--------------------------------
#first: ptr to the board
#second: dimensions
sub FindFirstUnsolved {
  my $board = shift();
  my $dimen = shift();

  my $i = 0;
  my $solved = 1;

  do {
    $j = 0;
    do {
      $solved = IsSolved(At($board, $i, $j, $dimen));
      $j++;
    }  while (($j < $dimen) && ($solved == 1));
    $i++;
  } while (($i < $dimen) && ($solved == 1));

  #save these for main
  $row = $i - 1;
  $col = $j - 1;
}


#--------------------------------
# At()
#   Returns the complete string from the array.
# Parameters:
#   1. address of the board
#   2. row to access
#   3. column to access
#   4. dimension
#--------------------------------
sub At {
  my $board = shift();
  my @board = @$board;
  my $row = shift();
  my $col = shift();
  my $dimen = shift();
 
  # watch boundaries 
  if (($row >= $dimen) || ($col >= $dimen)) {
    print "\tout of bounds access in At(), row = " . $row . "; col = ";
    print $col . "\n";
    return;
  }

  # calculate the index
  my $index = $row * $dimen;
  $index += $col;   

  # trying to get board[row][col]...
  my $dataDump = $board[($row * $dimen) + $col];

  return $dataDump;
}
  

#--------------------------------
# IsSolved()
#   Given a string, it checks to 
# see if that box is solved or not
# by checking for the presence of
# a pip value (integer).
# Parameters:
#   1. string
#--------------------------------
sub IsSolved {
  my $dump = shift();  
  
  # then there are 9 possible pip values.
  if ($dump !~ m/([\d])/g) {   
    return 0;
  }
  else {
    my $num = $1;  
    return 1;
  }      
}
