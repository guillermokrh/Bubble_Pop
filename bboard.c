/* bboard.c
* Name: Guillermo K. Rojas Hernandez
* NetID: grojas5
* Assignment: HW5
* Version: 12
* Description: implementation of bboard.h
*/

#include <stdio.h>
#include "bboard.h"
#include <stdlib.h>
#include <time.h>
#include "stack.h"


//define the struct from bboard.h
struct bboard {
     //keep track of the current score
	int curr_score;

     //stores board ptr (address of game board) 
     char board_array_ptr[MAX_ROWS][MAX_COLS];

     char balloon_colors[5];
     
     //stores how many rows there are in the board
     int num_rows;
     //stores how many cols there are in the board
     int num_cols;

     //Add a pointer to the stack that keeps track of undo operations
     StackPtr stack_ptr;  
     
};


/*
* bb_create
* populates board with random balloons
* return: pointer to newly created board with given nums of rows and cols
*/
BBoardPtr bb_create(int nrows, int ncols){

     if ((nrows > MAX_ROWS-1) || (ncols > MAX_COLS-1) || (nrows < 1)
          || (ncols < 1)) {
          fprintf(stderr, "Call to create is out of range. MAX is 40x40 ");
          fprintf(stderr, "and MIN is currently 1X1.\n");
          return NULL;
     }

     //allocate space for board
     BBoardPtr b = malloc(sizeof(struct bboard));

     //debug
     //random seed using time
     //int seed = 5;
     //srand((unsigned)seed);
     //fprintf(stderr, "rand val: %u\n", (rand() % 4 ) + 1);

     //initialize seed using time randomizer
     srand((unsigned)time(NULL));

     int i = 0; //indexing 
     int j = 0; //indexing


     //set initial score to 0
     b->curr_score = 0;
     
     //set row & col boundaries
     b->num_rows = nrows;
     b->num_cols = ncols;

     //set color types in balloon_colors array
     b->balloon_colors[0] = None;
     b->balloon_colors[1] = Red;
     b->balloon_colors[2] = Blue;
     b->balloon_colors[3] = Green;
     b->balloon_colors[4] = Yellow;

     //fill all cols and rows with random baloons
     for(i = 0; i < b->num_rows; i++){
          for (j = 0; j < b->num_cols; j++){
               //set all rows to random
               b->board_array_ptr[i][j] = b->balloon_colors[(rand() % 4)+1]; 
          }
     }

     //debug
     /*Print Address of the Array Location*/
     //fprintf(stderr, "Address of array: %p\n", b->board_array_ptr);


     /* INITIALIZE STACK ASSOCIATED WITH BOARD */
     b->stack_ptr = stk_create();


     //debug
     //printf("Address of b in bb_create: %p\n", b);
     //printf("Itm at board_array_ptr[0][4]: %i\n", b->board_array_ptr[0][4]);
     return b;
}

/*
* bb_create_from_mtx
* populates board contents of given 2D matrix (assumes given dimensions)
* return: pointer to newly created board with given nums and cols
*/
BBoardPtr bb_create_from_mtx(char mtx[][MAX_COLS], int nrows, int ncols){

     //driver checks the entered dimensions of the grid
     BBoardPtr b = malloc(sizeof(struct bboard));

     b->num_rows = nrows;
     b->num_cols = ncols;


     int i = 0;
     int j = 0;

     //copy board from mtx 2D array to the board struct's 2D array
     for(i = 0; i < b->num_rows; i++){
          for(j = 0; j < b->num_cols; j++){
               //debug
               //fprintf(stderr, "matrix char at %i,%i: %c", i, j, mtx[i][j]);
               b->board_array_ptr[i][j] = mtx[i][j];
          }
     }

     //debug
     //printf("mtx[0][0]: %c\n", mtx[0][0]);
     bb_display(b);

     /* INITIALIZE STACK ASSOCIATED WITH BOARD */
     b->stack_ptr = stk_create();

     return b;
}

/**
* bb_duplicate
* Helper function for bb_pop.
* Duplicates the state of the board that is passed in, in order to pass 
* it to the undo stack for future undo operations.  Allocates a new 
* address in memory for the duplicate.
*
* parameters: pointer to board that will be duplicated
* return: address to the new duplicated board 
*/

BBoardPtr bb_duplicate_for_stack(BBoardPtr b){

     BBoardPtr copy_b = malloc(sizeof(struct bboard));

     //copy # of rows, cols
     copy_b->num_rows = b->num_rows;
     copy_b->num_cols = b->num_cols;

     int i = 0;
     int j = 0;

     //copy board contents from original to copy
     for(i = 0; i < copy_b->num_rows; i++){
          for(j = 0; j < copy_b->num_cols; j++){
               //debug
               //fprintf(stderr, "matrix char at %i,%i: %c", i, j, mtx[i][j]);
               copy_b->board_array_ptr[i][j] = b->board_array_ptr[i][j];
          }
     }

     //copy the score
     copy_b->curr_score = b->curr_score;
     
     //set stack_ptr to board_array's stack_ptr
     copy_b->stack_ptr = b->stack_ptr; 


     /* //debug
     fprintf(stderr, "Displaying Duplicate of Board (will go in pop)\n");
     //display copy
     bb_display(copy_b);
     fprintf(stderr, "End of Displaying Duplicate (will go in pop)\n");
     */

     //return address of the copy
     return copy_b;
}

/** 
* bb_restore
* Helper function for bb_undo.  Restores the settings of a previous board 
* that was pushed to the undo stack.  
* parameters: BBoard pointer b (current playable board, to which we want to
*                   restore previous settings)
*              BBoard pointer b_undo (previous board that was pushed to the
*                   undo stack; we will copy the settings from this board)
* return: pointer to b, which now has restored settings. 
*/
BBoardPtr bb_restore(BBoardPtr b, BBoardPtr b_undo){

     //copy # of rows, cols
     //this should stay the same
     b->num_rows = b_undo->num_rows;
     b->num_cols = b_undo->num_cols;

     int i = 0;
     int j = 0;

     //copy board contents from original to copy
     for(i = 0; i < b->num_rows; i++){
          for(j = 0; j < b->num_cols; j++){
               //debug
               //fprintf(stderr, "matrix char at %i,%i: %c", i, j, mtx[i][j]);
               b->board_array_ptr[i][j] = b_undo->board_array_ptr[i][j];
          }
     }

     b->curr_score = b_undo->curr_score;

     //this should stay the same
     b->stack_ptr = b_undo->stack_ptr; //set stack_ptr to board_array's stack_ptr

     /* //debug
     fprintf(stderr, "Displaying Restored Board (result of undo)\n");
     //display copy
     bb_display(b);
     fprintf(stderr, "End of Displaying Result of undo\n");
     */

     //return address of the copy
     return b;
}


/**
* bb_destroy
* deallocate all dynamically allocated memory associated with board
* return: void
*/
void bb_destroy(BBoardPtr b){

     //check if stack is empty     
     while (!stk_is_empty(b->stack_ptr)){ //if not empty
          //pop stack and store BBoardPtr as temp
          BBoardPtr temp = stk_pop(b->stack_ptr);
          //debug
          //bb_display(temp);
          //fprintf(stderr, "& of temp's board: %p", temp->board_array_ptr);
          
          //free the BBoardPtr
          free(temp);
          //debug
          //fprintf(stderr, "freed board\n");
     }
     

     stk_free(b->stack_ptr); //free the stack allocated in board_create
     free(b); //free the main board pointer

}

/*
* bb_display
* print the board, drawing boundaries, and showing the symbols
* label rows and columns
* return: void
*/
void bb_display(BBoardPtr b){

     int i = 0; //for indexing
     int j = 0; //for indexing

     //debug
     /*
     printf("board->num_rows: %i\n", b->num_rows);
     printf("board->num_cols: %i\n", b->num_cols);
     printf("b->board_array_ptr address: %p\n", b->board_array_ptr);
     */

     /** TOP BORDER **/
     printf("  + ");
     for(i = 0; i < b->num_cols; i++){
          if (i != b->num_cols-1){
               printf("---");
          } else {
               printf("- "); //for last iteration change spaces
          }
     }
     printf("+");
     printf("\n"); //end of top border
     /** END OF TOP BORDER **/

     /**BOARD CONTENTS**/
     for (i = 0; i < b->num_rows; i++){
          //printf("in b->num_rows[i]\n");
          for (j = 0; j < b->num_cols; j++){
               //printf("in b->num_rows[j]\n");
               //if printing the first column
               if (j == 0) {
                    //fprintf(stderr, "test\n");
                    fprintf(stderr, "%i", i / 10); //print first digit
                    fprintf(stderr, "%i", i % 10); //print second digit
                    fprintf(stderr, "|"); //print left-most border

               }
               printf(" %c ", b->board_array_ptr[i][j]);
          }
          printf("|"); //print right-most border
          printf("\n"); // end of row, new line
     }
     
     /**END OF BOARD CONTENTS**/

     /** BOTTOM BORDER **/
     printf("  + ");
     for(i = 0; i < b->num_cols; i++){
          if (i != b->num_cols-1){
               printf("---");
          } else {
               printf("- "); //for last iteration change spaces
          }
     }
     printf("+");
     printf("\n"); //end of bottom border
     /** END OF BOTTOM BORDER **/

     /*COL NUMBERS*/
     printf("  ");
     for (i = 0; i < b->num_cols; i++){
               printf("  %i", i / 10);
     }
     printf("\n");

     printf("  ");
     for (i = 0; i < b->num_cols; i++){
               printf("  %i", i % 10);
     }
     //printf(" ");
     printf("\n\n"); //2newlines

     //debug
     //printf("done displaying\n");
}

/* in_bounds
*  Helper function for bb_pop
*  Parameters: pointer to board b, user entered coordinates (r, c) 
*  Returns 1 if coordinate is in bounds of board dimensions
*  Returns 0 if coordinate is out of bounds board dimensions
*/
int in_bounds(BBoardPtr b, int r, int c){

     if( ((r < 0)||(r > b->num_rows-1)) || ((c < 0)||(c > b->num_cols-1)) ){
          //debug
          /*fprintf(stderr, "in bounds FAIL: " 
                    "(r: %i, c: %i) out of bounds.\n", r, c); */
          //fprintf(stderr, "   return 0\n");
          return 0;
     }
     return 1;

}


/* bb_check_pop
* Helper function for bb_pop
* checks if it is possible for (r, c) to pop in north, south, east, west
* directions (if there is a match in any of those directions)
* returns more than 1 if it is possible to pop
* returns -1 if no matches in any directions
*/
int bb_check_pop(BBoardPtr b, int r, int c){
    
     int curr_balloon_type = bb_get_balloon(b, r, c);

     //debug
     //fprintf(stderr, "current balloon type: %c\n", curr_balloon_type);

     int north = r-1;
     int south = r+1;
     int west = c-1;
     int east = c+1;

     int north_south[2] = {north, south};
     int east_west[2] = {west, east};
     int i = 0; //index into directions_array
     //stores current balloon type 

     //check for east-west matches 
     for(i = 0; i < 2; i++){

          //check north-south matches
          if (in_bounds(b, north_south[i], c) == 0) { //if out of bounds north
               //debug
               //fprintf(stderr, "%i  out of bounds n/s.\n", north_south[i]);
               continue; //move on to next i
          }

          //returns coordinate of where match found
          if (b->board_array_ptr[north_south[i]][c] == curr_balloon_type){
               //debug
               //fprintf(stderr, "north_south match at (r:%i, c:%i): %c" 
               //"\n", north_south[i], c, b->board_array_ptr[north_south[i]][c]);
               return north_south[i];
          } else {
               //debug
               //fprintf(stderr, "No north-south match at (r:%i, c:%i): %c" 
               //"\n", north_south[i], c, b->board_array_ptr[north_south[i]][c]);
          }
     }


     //check for east-west matches 
     for(i = 0; i < 2; i++){

          //check east-west matches
          if (in_bounds(b, r, east_west[i]) == 0) { //if out of bounds north
               //debug
               //fprintf(stderr, "%i is o of bounds e/w.\n", east_west[i]);
               continue; //move on to next i
          }

          //returns coordinate of where match found
          if (b->board_array_ptr[r][east_west[i]] == curr_balloon_type){
               //debug
               //fprintf(stderr, "east-west at (r:%i, c:%i): %c" 
               //"\n", r, east_west[i], b->board_array_ptr[r][east_west[i]]);
               return east_west[i];
          } else {
               //debug
               //fprintf(stderr, "No east-west match at (r:%i, c:%i): %c" 
               //"\n", r, east_west[i], b->board_array_ptr[r][east_west[i]]);
          }
     }

     return -1;
}

/* recurse_all_ways
* Helper function for bb_pop
* Checks to see if there are matching balloons in any direction, pops
* them accordingly.
* return: void
*/
void recurse_all_ways(BBoardPtr b, int r, int c, int* counter_ptr, char btype){

     //BASE CASES
     //if not in bounds return
     if (in_bounds(b, r, c) == 0){
          return;
     }
     //if types do not match, return
     if (b->board_array_ptr[r][c] != btype){
          return;
     }

     //we are in bounds and the type matches
     //make the current r,c none
     b->board_array_ptr[r][c] = None;
     (*counter_ptr)++; //increase the value at counter_ptr

     //otherwise, we are in bounds and the type matches
     //recurse east
     recurse_all_ways(b, r, c+1, counter_ptr, btype);
     //recurse west
     recurse_all_ways(b, r, c-1, counter_ptr, btype);
     //recurse north
     recurse_all_ways(b, r-1, c, counter_ptr, btype);
     //recurse south
     recurse_all_ways(b, r+1, c, counter_ptr, btype);

     return; //return function
 
}



/*bb_pop
* attempts to pop balloon at (r,c) and its cluster
* adds n*(n-1) to score if balloons were popped
* return: # of balloons popped 
* STATUS: not done
*/
int bb_pop(BBoardPtr b, int r, int c){

     //for less confusion
   
     int user_r = r;
     int user_c = c;

     int i = 0;
     int j = 0;

     //counts how many balloons popped
     int counter = 0;
     int* ptr_to_counter = &counter;

     //check for in bounds
     if (in_bounds(b, user_r, user_c) == 0) {
          fprintf(stderr, "user query (user_r: %i, user_c: %i) is " 
                    "out of bounds.\n", user_r, user_c);
          fprintf(stderr, "bb_pop to return 0. Goodbye.\n");
          return counter; 
     }

     //check if location is empty
     if(b->board_array_ptr[user_r][user_c] == None){
          fprintf(stderr, "user query (user_r: %i, user_c: %i) is " 
                    "empty.\n", user_r, user_c);
          fprintf(stderr, "bb_pop to return 0. Goodbye.\n");
          return counter;
     }

     //check if location is any character other than the balloon types
     if ( (b->board_array_ptr[user_r][user_c] != Red) &&
          (b->board_array_ptr[user_r][user_c] != Blue) &&
          (b->board_array_ptr[user_r][user_c] != Green) &&
          (b->board_array_ptr[user_r][user_c] != Yellow) ) {

          fprintf(stderr, "user query (user_r: %i, user_c: %i) is " 
                    "not a valid balloon color.\n", user_r, user_c);
          fprintf(stderr, "bb_pop to return 0. Goodbye.\n");
          return counter;

          }

     //check if it is possible to pop in any direction
     if (bb_check_pop(b, user_r, user_c) == -1) {
          //-1 means there are no matches in any direction
          return counter;
     }


     //at this point we know there is at least one match for this coordinate

     //before popping balloons, push a copy of this instance to the stack
     int push_success = stk_push(b->stack_ptr, bb_duplicate_for_stack(b));
     //debug
     //fprintf(stderr, "push success: %i", push_success);

     //stores initial balloon type
     char balloon_type = b->board_array_ptr[user_r][user_c];
     //debug
     //fprintf(stderr, "balloon type: %c\n", balloon_type);

     //pop in all directions from the initial balloon
     recurse_all_ways(b, r, c, ptr_to_counter, balloon_type);

     //Tallies up score from popped ballon
     b->curr_score = b->curr_score + (counter*(counter-1));
     //debug
     //fprintf(stderr, "counter amount: %i\n", counter);

     //debug
     int stack_size = stk_size(b->stack_ptr);
     fprintf(stderr, "stack size: %i\n", stack_size);
     
	//return # of balloons popped
     return counter;
}

/*
* bb_is_compact
* determines if balloons are as "high" as possible (if and only if
* all slots in the column above it are also balloons (non-empty)
* return: 1 if as high as possible, 0 otherwise
*/
int bb_is_compact(BBoardPtr b){

     //indexing
     int i = 0;
     int j = 0;

     for (i = 0; i < b->num_rows; i++){
          for (j = 0; j < b->num_cols; j++){
               
               /* //debug
               fprintf(stderr, "%i,%i: ", i, j);
               fprintf(stderr, "%c, ", b->board_array_ptr[i][j]);
               fprintf(stderr, "%c\n", b->board_array_ptr[i+1][j]);
               */
               //if not the last row, and if the item at index i, j is none
               if ((i != b->num_rows-1) && (b->board_array_ptr[i][j]==None)
                    && (b->board_array_ptr[i+1][j] != None)){
                    return 0; 
               }
          }
     }

     //if all checks pass, board is compact
     return 1; 
}

/*
* bb_float_one_step
* Move all balloons that are not as high as possible up one step
* Use animation to display each intermediate configuration
* return: 1 if successful
*/
int bb_float_one_step(BBoardPtr b){

     //indexing
     int i = 0;
     int j = 0;

     char swap = None; //initial value of swap set to none
    
     //display the board before making moves to make the board compact
     //debug
     //bb_display(b);

     //we are here under the assumption that the board is not compact
     for (j = 0; j < b->num_cols; j++){
          for (i = 0; i < b->num_rows; i++){
              
               /*  //debug
               fprintf(stderr, "%i,%i: ", i, j);
               fprintf(stderr, "%c, ", b->board_array_ptr[i][j]);
               fprintf(stderr, "%c\n", b->board_array_ptr[i+1][j]);
               */
               
               //if not the last row, and if the item at index i, j is none
               if ((i != b->num_rows-1) && (b->board_array_ptr[i][j]==None)
                    && (b->board_array_ptr[i+1][j] != None)){
                    //return 0; 
                    //instead of removing all, move [i+1][j] up one
                    //there is nothing there, so don't need to swap it,
                    //but swapping might actually be best, so its a 1 to 1
                    //change
                    swap = b->board_array_ptr[i][j];
                    b->board_array_ptr[i][j] = b->board_array_ptr[i+1][j]; 
                    b->board_array_ptr[i+1][j] = swap;


                    //debug
                    //bb_display(b);
               }
          }
     }
    

	//return 1 or 0
     return 1; //temporary return
}

/*
* bb_score
* Gets the current score
* return: the current score
*/
int bb_score(BBoardPtr b){
     //return: the current score
     return b->curr_score;
}

/*
* bb_get_balloon
* return: - color of the balloon at given location as a char
*		- if (r,c) is out of range, -1 is returned
*/

int bb_get_balloon(BBoardPtr b, int r, int c){

     //check if (r, c) is out of range
     if (r > b->num_rows || c > b->num_cols){
          //debug
          //fprintf(stderr, "Out of Range");
          return -1; //works
     }

     //otherwise, if no return, is in range
     return b->board_array_ptr[r][c]; //works
}

/*
* bb_can_pop
* Checks if there are balloons left to pop: if board is empty, nothing
*	is left to pop, if no balloons of same color are adjacent, same
* return: 1 if the player can pop some balloons, otherwise, 0
*/
int bb_can_pop(BBoardPtr b){

     int i = 0;
     int j = 0;

     for(i = 0; i < b->num_rows; i++){
          for(j = 0; j < b->num_cols; j++){
               //if balloon is blank, go to next column
               if (bb_get_balloon(b, i, j) == None) {
                    //debug
                    //fprintf(stderr, "balloon spot is empty\n");
                    continue;
               }
               //check pop returns -1 if can't pop anywhere
               if (bb_check_pop(b, i, j) != -1){
                    //debug                             
                    //fprintf(stderr, "board can pop " 
                    //          "at (r:%i, c:%i)\n", i, j);
                    return 1; //return 1 if balloon can pop
               }
               //fprintf(stderr, "board can't pop" 
               //          "at (r:%i, c:%i)\n", i, j);
          }
     }

     //fprintf(stderr, "Board can't pop at all.");
     return 0; //temporary return
}


/*
* bb_undo
* "Undoes" most recent balloon pops, & all intermediate "float" operations
* Restores score to previous value
* Return: 1 if successful, 0 otherwise
*/
int bb_undo(BBoardPtr b){

     //Check if there are any undos to make
     if(stk_size(b->stack_ptr) == 0){
          return 0;
     }

     //Otherwise, pop the last addition to the stack
     //Store address
     BBoardPtr last_move = stk_pop(b->stack_ptr);

     //restore values from that last move to the current board pointer
     bb_restore(b, last_move); 

     //free the last move
     free(last_move);

     //display the current board
     //bb_display(b);

     return 1; //temporary return
}

