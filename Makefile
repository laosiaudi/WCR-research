
objects = query.o

main : ${objects}	
	g++ -g -o main ${objects}

query.o : query.cpp
	g++ -g -c query.cpp
