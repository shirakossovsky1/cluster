FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LIBS = -lm

all: main.o adjacency_matrix.o modularity_matrix.o leading_eigenpair.o power_iteration.o maximize_modularity.o division.o utils.o linear_algebra_utils.o error.o
	gcc main.o adjacency_matrix.o modularity_matrix.o leading_eigenpair.o power_iteration.o maximize_modularity.o division.o utils.o linear_algebra_utils.o error.o -o cluster $(LIBS)
clean:
	rm -rf *.o cluster

main.o: main.c leading_eigenpair.h division.h utils.h 
	gcc $(FLAGS) -c main.c
	
adjacency_matrix.o: utils.h error.h adjacency_matrix.h
	gcc $(FLAGS) -c adjacency_matrix.c

modularity_matrix.o: adjacency_matrix.h modularity_matrix.h
	gcc $(FLAGS) -c modularity_matrix.c
	
leading_eigenpair.o: modularity_matrix.h power_iteration.h linear_algebra_utils.h leading_eigenpair.h
	gcc $(FLAGS) -c leading_eigenpair.c
	
power_iteration.o: leading_eigenpair.h utils.h linear_algebra_utils.h power_iteration.h
	gcc $(FLAGS) -c power_iteration.c

maximize_modularity.o: leading_eigenpair.h linear_algebra_utils.h maximize_modularity.h 
	gcc $(FLAGS) -c maximize_modularity.c
		
division.o: utils.h division.h
	gcc $(FLAGS) -c division.c
	
utils.o: modularity_matrix.h error.h utils.h
	gcc $(FLAGS) -c utils.c

linear_algebra_utils.o: modularity_matrix.h linear_algebra_utils.h
	gcc $(FLAGS) -c linear_algebra_utils.c
	
error.o: error.h
	gcc $(FLAGS) -c error.c
	
	