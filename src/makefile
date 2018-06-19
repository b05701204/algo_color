# CC: which compile to use
CC = g++-8
# variable setting
CFLAGS = -c
DBGFLAGS = -Wall -g 
OPTFLAGS = -O2 
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging version
# -O2 option is for optimized version

all	: main
	@echo -n ""

# link the code
main: main.o
	$(CC) $(DBGFLAGS) -o main main.o 
# compile the source code
main.o: main.cpp nets.h Routing.h util.h trace.h
	$(CC) $(DBGFLAGS) -c main.cpp

# Routing.o: Routing.h nets.h trace.h util.h
# 			$(CC) $(OPTFLAGS) Routing.h nets.h trace.h util.h > Routing.o

# Rout.o	: main.cpp nets.h Routing.o
# 			$(CC) $(OPTFLAGS) main.cpp nets.h Routing.o  Rout.o

# clean all the .o and executable files
clean:
		rm -rf *.o *.gch main

