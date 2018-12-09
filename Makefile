all: main.o
	g++ -std=c++11 $^ -o prog

#implicit rule	
#$< variable denotes the first dependency, %.cpp here.
%.o : %.cpp %.hpp
	g++ -std=c++11 -c $<

#other variables, for information:
#$@ the target
#$^ list of all the dependencies (including the first one, denoted by $<)

	
run:
	./prog
	
clean:
	rm *.o prog

#reminder, the format of a rule is
#
#target: dependency(ies)
#	command(s)
#
#(the second line has to start with a TABULATION)
