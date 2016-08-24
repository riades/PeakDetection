FLAGS  = -std=c++11

all: main

main: main.o cwt.o findlocalmaxima.o findridges.o identifymajorpeaks.o
	g++ main.o cwt.o findlocalmaxima.o findridges.o identifymajorpeaks.o -o main $(FLAGS)

main.o: main.cpp
	g++ -c main.cpp $(FLAGS)

cwt.o: cwt.cpp cwt.h
	g++ -c cwt.cpp $(FLAGS)

findlocalmaxima.o: findlocalmaxima.cpp findlocalmaxima.h 
	g++ -c findlocalmaxima.cpp $(FLAGS)

identifymajorpeaks.o: identifymajorpeaks.cpp identifymajorpeaks.h ridge.h
	g++ -c identifymajorpeaks.cpp $(FLAGS)

findridges.o: findridges.cpp findridges.h ridge.h
	g++ -c findridges.cpp $(FLAGS)

clean:
	rm -rf *.o main