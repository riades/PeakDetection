#include <iostream>
#include <numeric>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

#include "cwt.h"
#include "findlocalmaxima.h"
#include "identifymajorpeaks.h"
#include "findridges.h"

int main() {
	std::string line;
  	std::vector<double> vec;
  	std::ifstream test_file ("data/test.txt");
  	if (!test_file.good()) {
    	return 0;
  	}
  	while (getline(test_file,line)) {
    	std::stringstream stream(line);
    	double mass, intensity;
    	stream >> mass >> intensity;
    	vec.push_back(intensity);
  	}
	std::vector<double> scales(63);
	std::iota(std::begin(scales), std::end(scales), 1);
	CWT mexhCWT;
	std::vector<std::vector<double> > cwtVec = mexhCWT.compute(vec, scales);
	std::vector<std::vector<bool> > result = findLocalMaxima(cwtVec, scales, 5);
	std::vector<Ridge> ridges = findRidges(result, scales);
	std::vector<size_t> peaks = identifyMajorPeaks(cwtVec, scales, ridges);
	for (size_t k: peaks) {
		std::cout << k << ", ";
	}
}
