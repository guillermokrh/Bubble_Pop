
simpleio.o: simpleio.c simpleio.h ;clang -c simpleio.c

stack.o: stack.c stack.h ;clang -c stack.c

#BPOP_MAIN
#NOTE: b_pop works well and recursively in this file
bboard.o: bboard.c bboard.h stack.o ;clang -c bboard.c

#driver is using bboard_12.c
bpop: bpop.c bboard.o simpleio.o stack.o ;clang bpop.c -o bpop bboard.o simpleio.o stack.o
