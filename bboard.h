
#define MAX_ROWS 40
#define MAX_COLS 40


/** incomplete types **/
typedef struct bboard BBoard;
typedef struct bboard *BBoardPtr;

//NOTE: Changed None from ' ' to '.'
#define None '.'
#define Red  '^'
#define Blue '='
#define Green 'o'
#define Yellow '+'


/**
* Function:  bb_create
* Parameters: number of rows and columns
* Description: returns pointer to newly created
*              board with given number of rows
*              and columns.
*
*              Populates board with random balloons.
*
*              If nrows or ncols is out of range, 
*              an error message is printed to stderr
*              and NULL is returned.
*
*/
extern BBoardPtr bb_create(int nrows, int ncols);

/**
* Function:  bb_create_from_mtx
* Parameters: number of rows and columns and 2D array of char
* Description: returns pointer to newly created
*              board with given number of rows
*              and columns.
*
*              Populates board contents of given 2D matrix (assumes
*	       given dimensions).
*
*              If nrows or ncols is out of range, 
*              an error message is printed to stderr
*              and NULL is returned.
*
*/
extern BBoardPtr bb_create_from_mtx(char mtx[][MAX_COLS], int nrows, int ncols);

/**
* Function:  bb_clear
* Description:  sets all slots to be empty (None)
*               sets score to zero
*
*/
/* Prof. Lillis has deemed this to be not necessary
extern void bb_clear(BBoardPtr b);
*/

/**
* Function:  bb_destroy
* Description:  deallocates all dynamically allocated 
*  		memory associated with the board.
*		
*		Comment:  this might start out as
*		a one-liner.
*
*/
extern void bb_destroy(BBoardPtr b);


/**
* Function:  bb_display
* Description:  pretty-prints the current board.
*
*		uses '|', '+' and '-' to "draw" the boundaries
*               of the board.
*
*		For full credit rows and columns are labeled with
* 		their indices.  Column indices displayed "vertically"
*
*		Examples:
*
*  This board is full of balloons (like at the beginning of a game).
* 
*             +-----------------------+
*		  0 | + + + = + ^ ^ o + = = |
*		  1 | ^ = = o + o + + + ^ = |
*		  2 | ^ + ^ o + o + = = = = |
*		  3 | = ^ o o = o + + + = = |
*		    +-----------------------+
* 		      0 0 0 0 0 0 0 0 0 0 1
*		      0 1 2 3 4 5 6 7 8 9 0
*
*
*  This board has some empty slots represented by  '.'
*             +-----------------------+
*		  0 | + + + = + ^ ^ o + = = |
*		  1 | ^ = o o + o + + + ^ = |
*		  2 | ^ + . o + . + = . . = |
*		  3 | . . . o . . + + . . = |
*		    +-----------------------+
* 		      0 0 0 0 0 0 0 0 0 0 1
*		      0 1 2 3 4 5 6 7 8 9 0
*
*
*  Here is the same board using a space ' ' to 
*  represent an empty slot.
*
*             +-----------------------+
*		  0 | + + + = + ^ ^ o + = = |
*		  1 | ^ = o o + o + + + ^ = |
*		  2 | ^ +   o +   + =     = |
*		  3 |       o     + +     = |
*		    +-----------------------+
* 		      0 0 0 0 0 0 0 0 0 0 1
*		      0 1 2 3 4 5 6 7 8 9 0
*
*/
extern void bb_display(BBoardPtr b);

/**
* Function:  bb_pop
* Description:  (attempts to) "pop" the balloon at (r,c) (if any)
*               and its associated "color cluster"
*
*               Cluster must be of AT LEASE SIZE 2 (a balloon that
*   		is its own cluster will not be popped).
*
* Returns:  number of balloons popped
*
* Side effect:  adds n*(n-1) to score if n balloons were popped.
*
*/
extern int bb_pop(BBoardPtr b, int r, int c);

/**
* Function:  bb_is_compact
* Description:  determines if all balloons are as "high" as possible.
*               Returns 1 if this is the case
*               Returns 0 otherwise.
*
*               Note:  a balloon is as high as possible if and only
*     		if all slots in its column ABOVE it are also balloons
*		(non-empty)
*
*/
extern int bb_is_compact(BBoardPtr b);

/**
* Function:  bb_float_one_step
* Description:  moves all balloons that that are NOT AS HIGH AS POSSIBLE
*               up one spot.  
*
*   Example:
*
*		before
*
*                   +-----------------------+
*		  0 | + + + = + ^     + = = |
*		  1 | ^       + o + +   ^ = |
*		  2 | ^ +   o +   + =     = |
*		  3 |       o     + + =   = |
*		    +-----------------------+
* 		      0 0 0 0 0 0 0 0 0 0 1
*		      0 1 2 3 4 5 6 7 8 9 0
*
*		after
*
*                   +-----------------------+
*		  0 | + + + = + ^ + + + = = |
*		  1 | ^ +   o + o + =   ^ = |
*		  2 | ^     o +   + + =   = |
*		  3 |                     = |
*		    +-----------------------+
* 		      0 0 0 0 0 0 0 0 0 0 1
*		      0 1 2 3 4 5 6 7 8 9 0
*
*
* Note:  think animation by repeating this until it
*   is compact (perhaps displaying each intermediate
*   configuration).
*/
extern int bb_float_one_step(BBoardPtr b);

/**
* Function:  bb_score
* Description:  returns the current score 
*
*/
extern int bb_score(BBoardPtr b);


/**
* Function:   bb_get_balloon
* Description:  returns color of balloon at given location as a char.
*		if (r,c) is out of range, -1 is returned.
*
*/
extern int bb_get_balloon(BBoardPtr b, int r, int c);


/**
* Function:   bb_can_pop
* Description:  returns 1 if the player can pop some balloons;
*		        0 otherwise.
*
*		If the board is empty, of course nothing can
*			be poppsed.
*		If there are no adjacent balloons of the same
*		color, there also are no moves.
*	
* Notes:  probable usage is to determine when game is over.
*/
extern int bb_can_pop(BBoardPtr b);


/**
* Function:  bb_undo
* Description:  "undoes" most recent pop (if any).
*               includes all intermediate "float" operations.
*
*               Restores score to corresponding previous value.
*
*               Returns 1 if successful; 0 otherwise (no previous config)
*
*/
extern int bb_undo(BBoardPtr b);

