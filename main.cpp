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
  	std::string fileName; 
  	getline(std::cin, fileName);
  	std::ifstream testFile (fileName);
  	if (!testFile.good()) {
    	return 0;
  	}
  	while (getline(testFile,line)) {
    	std::stringstream stream(line);
    	double mass, intensity;
    	stream >> mass >> intensity;
    	vec.push_back(intensity);
  	}
	std::vector<double> scales(63);
	std::iota(std::begin(scales), std::end(scales), 1);
	std::vector<std::vector<double> > cwtVec = cwt(vec, scales);
	std::vector<std::vector<bool> > result = findLocalMaxima(cwtVec, scales, 5);
	std::vector<Ridge> ridges = findRidges(result, scales);
	for (auto ridge: ridges) {
		for (auto val: ridge.positions) {
			std::cout << val << " ";
		}
		std::cout << std::endl;
	}
	std::vector<size_t> peaks = identifyMajorPeaks(cwtVec, scales, ridges);
	for (size_t k: peaks) {
		std::cout << k << ", ";
	}
}
