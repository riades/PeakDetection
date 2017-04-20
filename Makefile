FLAGS  = -std=c++11
OPTS = -lfftw3 -lfftw3_threads -lm
FLM = findlocalmaxima
FR = findridges
IMP = identifymajorpeaks
all: main

main: main.o cwt.o $(FLM).o $(FR).o $(IMP).o
	g++ main.o cwt.o $(FLM).o $(FR).o $(IMP).o -o main $(FLAGS) $(OPTS) 

main.o: main.cpp
	g++ -c main.cpp $(FLAGS)

cwt.o: cwt.cpp cwt.h
	g++ -c cwt.cpp $(FLAGS)

$(FLM).o: $(FLM).cpp $(FLM).h 
	g++ -c $(FLM).cpp $(FLAGS)

$(IMP).o: $(IMP).cpp $(IMP).h ridge.h
	g++ -c $(IMP).cpp $(FLAGS)

$(FR).o: $(FR).cpp $(FR).h ridge.h
	g++ -c $(FR).cpp $(FLAGS)

clean:
	rm -rf *.o main