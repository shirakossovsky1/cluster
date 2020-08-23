FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LIBS = -lm

all: main.o input_matrix.o modularity_matrix.o leading_eigenpair.o power_iteration.o maximize_modularity.o utils.o division.o
	gcc main.o input_matrix.o modularity_matrix.o leading_eigenpair.o power_iteration.o maximize_modularity.o utils.o division.o -o run $(LIBS)
clean:
	rm -rf *.o run

main.o: main.c leading_eigenpair.h division.h utils.h 
	gcc $(FLAGS) -c main.c
	
input_matrix.o: utils.h input_matrix.h
	gcc $(FLAGS) -c input_matrix.c

modularity_matrix.o: input_matrix.h modularity_matrix.h
	gcc $(FLAGS) -c modularity_matrix.c
	
leading_eigenpair.o: modularity_matrix.h power_iteration.h leading_eigenpair.h
	gcc $(FLAGS) -c leading_eigenpair.c
	
power_iteration.o: leading_eigenpair.h power_iteration.h
	gcc $(FLAGS) -c power_iteration.c

maximize_modularity.o: leading_eigenpair.h maximize_modularity.h
	gcc $(FLAGS) -c maximize_modularity.c
	
utils.o: utils.h
	gcc $(FLAGS) -c utils.c
	
division.o: utils.h division.h
	gcc $(FLAGS) -c division.c
	