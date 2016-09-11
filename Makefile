FLAGS  = -std=c++11
OPTS = -lfftw3 -lfftw3_threads -lm
FLM = findlocalmaxima
FR = findridges
IMP = identifymajorpeaks
CONV = convolution/
FFTWW = fftwwrappers
FFTWC = fftwconvolve
all: main

main: main.o cwt.o $(FLM).o $(FR).o $(IMP).o $(FFTWW).o $(FFTWC).o
	g++ main.o cwt.o $(FLM).o $(FR).o $(IMP).o $(FFTWW).o $(FFTWC).o -o main $(FLAGS) $(OPTS) 

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

$(FFTWW).o: $(CONV)$(FFTWW).cpp $(CONV)$(FFTWW).h
	g++ -c $(CONV)$(FFTWW).cpp $(FLAGS)

$(FFTWC).o: $(CONV)$(FFTWC).cpp $(CONV)$(FFTWW).h
	g++ -c $(CONV)$(FFTWC).cpp $(FLAGS)

clean:
	rm -rf *.o main