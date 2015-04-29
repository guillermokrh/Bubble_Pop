/**
* Bubble Pop Game.
*
* Client of bboard module/ADT
*
* Implements Simple terminal interface.
*/

#include <stdio.h>
#include <stdlib.h>
#include "bboard.h"
#include "simpleio.h"
#include <sys/time.h>

// kludge:  symbol used in input files for "no balloon".
//          may not match symbol used by bboard
#define MY_NONE '.'

#define SLEEP_MS 200  // defines delay between "frames" in

// user commands
#define POP 1
#define UNDO 2
#define QUIT 3
#define UNKNOWN 4

static char Balloons[] = {MY_NONE, Red, Blue, Green, Yellow};

#define NCOLORS 5

/**
* makes program sleep the given number of milliseconds
*
* Citation:  adapted from stackoverflow:
*
*    http://stackoverflow.com/a/28827188
*/
void sleep_ms(int milliseconds) // cross-platform sleep function
{
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = milliseconds * 1000000;
    nanosleep(&ts, NULL);
}

void clear(){
  printf("\033c\n");
}

/**
* Eats whitespace from input stream fptr until either
*   a non-whitespace character is encountered or we
*   reach the end of file.
*   
* returns:  the non-whitespace char found or EOF if 
*    none exists.
*
* Note return type is int, rather than char
*/
int next_symbol(FILE *fptr) {
int ch;

  while(( (ch = fgetc(fptr)) != EOF) && isspace(ch));

  return ch;
}


int is_balloon(int ch) {
int i;
  for(i=0; i<NCOLORS; i++){
	if(ch == Balloons[i])
	  return 1;
  }
  return 0;
}


/**
* read_grid
*
* populates the given 2D matrix with "grid" stored in 
*   the given file.
* 
* number of rows/cols stored in *nrows_out, *ncols_out
*   respectively.
*
* returns 0/1 for success/failure
*/
int read_grid(char grid[MAX_ROWS][MAX_COLS], char *file, 
		  int *nrows_out, int *ncols_out) {
FILE *fptr;
// by the way:  stdin is a global var of type FILE *
int nrows, ncols;
int i, j;
int ch;

  // attempt to open file with 'read' permission
  fptr = fopen(file, "r");
  if(fptr == NULL) {
	fprintf(stderr, "ERR:  failed to open file %s\n", file);
	return 0;
  }
  // read the dimensions of the grid
  if(fscanf(fptr, "%i %i\n", &nrows, &ncols) != 2 ||
	  nrows <= 0 || ncols <= 0 || nrows > MAX_ROWS || ncols > MAX_COLS){
	fprintf(stderr, "ERR:  bad board dimensions\n");
	fclose(fptr);
	return 0;
  }
  // now populate the grid
  for(i=0; i<nrows; i++) {
	for(j=0; j<ncols; j++) {
	  if( (ch = next_symbol(fptr)) == EOF) {
		fprintf(stderr, "ERR:  file too short\n");
		fclose(fptr);
		return 0;
	  }
	  else if(!is_balloon(ch)) {
		fprintf(stderr, "ERR:  bad symbol in input (%c)\n", ch);
		fclose(fptr);
		return 0;
	  }
	  
	  grid[i][j] = ch;
	}
  }
  if(next_symbol(fptr) != EOF) {
	fprintf(stderr, "ERR:  file too long\n");
	fclose(fptr);
	return 0;
  }
  // if we get here we succeeded!
  fclose(fptr);
  *nrows_out = nrows;
  *ncols_out = ncols;
  return 1;
}


/**
* kludge:  we are using '.' to indicate "No Balloon" in input
*    files.  This may not match the symbol used by bboard so we
*    clean it up before sending the matrix off to bb_create_from_mtx
*/
void scrub_grid(char grid[][MAX_COLS], int nrows, int ncols) {
int i, j;

  for(i=0; i<nrows; i++) {
	for(j=0; j<ncols; j++) {
	    if(grid[i][j] == MY_NONE)
		grid[i][j] = None;
	}
  }
}


int read_cmd(int *pr, int *pc){
char *buf;
  
  printf(" -------------------------\n");
  printf(" enter <row> <col> to pop or u (undo) or q (quit): ");

  buf = get_string();
  if(*buf == '\0') {
	free(buf);
	return UNKNOWN;
  }
  if(*buf == 'q'){
	free(buf);
	return QUIT;
  }
  if(*buf == 'u'){
	free(buf);
	return UNDO;
  }
  if(sscanf(buf, "%i %i", pr, pc) == 2){
	free(buf);
	return POP;
  }
  else {
	free(buf);
	return UNKNOWN;
  }
}
  
void animate(BBoardPtr board) {
  
  do {
	sleep_ms(300);
	bb_float_one_step(board);
	clear();
	bb_display(board);
  }
  while(!bb_is_compact(board));
}

void make_compact(BBoardPtr board) {
  
  while(!bb_is_compact(board))
	bb_float_one_step(board);
}


int main(int argc, char *argv[]) {
BBoardPtr board=NULL;
char grid[MAX_ROWS][MAX_COLS];
int cmd;
int nrows, ncols, r, c, score;
int done;



    if(argc == 1) 
	board = bb_create(10, 10);
    else if(argc==2){
	if(!read_grid(grid, argv[1], &nrows, &ncols)) {
	    fprintf(stderr, "failed to read file %s\n", argv[1]);
	    fprintf(stderr, "Goodbye\n");
	    return 0;
	}
	printf("Successfully read input file %s\n", argv[1]);
	scrub_grid(grid, nrows, ncols);
	board = bb_create_from_mtx(grid, nrows, ncols);
     }
    else {
        fprintf(stderr, "usage error:  zero or 1 cmd-line arg\n");
        fprintf(stderr, "Goodbye\n");
	return 0;
    }
    if(board != NULL) {

	if(!bb_is_compact(board)) {
		fprintf(stderr, "ERR:  board not compact\n");
		bb_display(board);
		fprintf(stderr, "Goodbye\n");
		bb_destroy(board);
		return 1;
	}
	done = 0;
	while(!done) {
	    int npopped;
	    // bb_display(board);
	    animate(board);
	    score = bb_score(board);
	    printf("SCORE: %i\n", score);
	    cmd = read_cmd(&r, &c);
	    
	    switch(cmd) {
		case POP:  npopped = bb_pop(board, r, c); 
			   // fprintf(stderr, "npopped: %i\n", npopped);
			   // make_compact(board);
			   animate(board);
			   break;
		case QUIT: // bb_destroy(board);
			   done = 1;
			   break;
		case UNDO: 
                  //printf("undo not implemented\n");
			   sleep_ms(500);
			   
			   if(!bb_undo(board)) {
				printf("  undo failed\n");
			   }
			   
			   break;
		case UNKNOWN:
			   printf("  what?  try again\n");
			   break;
	
	    }
	    if(!bb_can_pop(board))  // no more moves for player
		done = 1;
	}
	printf("FINAL SCORE:  %i\n", bb_score(board));
	bb_destroy(board);
	printf("  Goodbye\n");
    }  // end if board != NULL
}
	
	
	    
	
	
	
