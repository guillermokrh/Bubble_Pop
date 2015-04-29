/* 
* stack.c
* Name: Guillermo K. Rojas Hernandez
* NetID: grojas5
* Assignment: HMW4 stack.c
* Version: 3
* Description: 
* 	Homework 4
*/

#include "stack.h"

struct stack_struct {
	int curr_capacity; //current capacity
	ElemType items[CAPACITY]; //init array
	int top; //stores index at top of the stack
	ElemType* items_ptr; //stores new location of array of double capacity
};


StackPtr stk_create(){
	StackPtr s = malloc(sizeof(struct stack_struct));

	//stack initially empty
	s->top = -1;  // by convention, set to -1 to indicate empty
	s->curr_capacity = CAPACITY; //initialize this to capacity
	s->items_ptr = s->items; //init items pointer to initial items array

    return s; //return pointer to address created by malloc
}


void stk_free(StackPtr s) {
    free(s);
}

int stk_push(StackPtr s, ElemType val) {

	//if reach capacity, double the size of the stack
	//and then push the rest of the contents
	if(s->top == s->curr_capacity - 1){

		//for indexing
		int i = 0;

		//double the capacity of the array is two times the current
		//capacity of the value at s->curr_capacity
		int dbl_capacity = (s->curr_capacity)*2;
		//allocate memory for double capacity
		ElemType* dbl_array = malloc(sizeof(ElemType)*dbl_capacity);


		for (i = 0; i < s->curr_capacity; i++){
			//copy all values into corresponding
			//index in the new doubled array
			dbl_array[i] = s->items_ptr[i];
		}

		//Set current capacity of s to double current capcity
		s->curr_capacity = (s->curr_capacity)*2;
		//Set the pointer of the previous array to the new array
		s->items_ptr = dbl_array;

	}

	//add items to the top of stack
	s->top++;
	s->items_ptr[s->top] = val;
	
	//return 1 if push function executes
	return 1;
}

ElemType stk_pop(StackPtr s){
    if(s->top == -1)
        abort();  // library function which terminates program!!!
    s->top--;
    return s->items_ptr[s->top+1];
}

int stk_is_full(StackPtr s){
    return 0; //returns 0 because stack is nvever full
}

int stk_is_empty(StackPtr s){
    return s->top == -1;
}

int stk_size(StackPtr s) {
    return s->top+1;
}

void stk_clear(StackPtr s){
    s->top = -1;
}

void stk_print(StackPtr s) {
    int i;
    printf("\n----TOP-----\n");

	//why starting with the top?
    for(i=s->top; i>=0; i--) {
        printf(FORMAT_STRING, s->items_ptr[i]);
    }
    printf("---BOTTOM---\n");


}

/*function: stk_clone
* parameters: pointer to stack
* returns address of new stack with the same contents and order
* as the stack passed in as a parameter.
*/
StackPtr stk_clone(StackPtr s) {

	//store master_capacity as the current capacity of the master stack
	int master_capacity = s->curr_capacity;
	int master_top = s->top;

	//allocate memory for an array associated with cloned stack
	ElemType* clone_array = malloc(sizeof(ElemType)*master_capacity);

	int i = 0; //index
	//copy contents from master stack to array that will be used
	//by the cloned stack
	for(i=0; i < master_capacity; i++){
		clone_array[i] = s->items_ptr[i];
	}

	//declare stack that will point to cloned contents
	StackPtr clone_stk;
	//initialize clone_stk 
	clone_stk = stk_create();

	//initialize struct values for cloned stack
	clone_stk->top = master_top; 
	clone_stk->curr_capacity = master_capacity;

	//set items_ptr to point to cloned array
	clone_stk->items_ptr = clone_array;

	//return  pointer to cloned stack
	return clone_stk; 
}

