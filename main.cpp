#include "cwt.h"
#include "findlocalmaxima.h"
#include "identifymajorpeaks.h"
#include "findridges.h"
#include <iostream>
#include <numeric>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

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
	std::vector<std::vector<double> > cwtVec(scales.size());// = cwt(vec, scales, &mexh);
	std::ifstream wcoefs_file("data/wcoefs.txt");
	if (!wcoefs_file.good()) {
    	return 0;
  	}
	for (size_t i = 0; i < cwtVec.size(); i++) {
		getline(wcoefs_file,line);
		std::stringstream stream(line);
		cwtVec[i].resize(vec.size());
		for (size_t j = 0; j < cwtVec[i].size(); j++) {
			double val;
			stream >> val;
			cwtVec[i][j] = val;
		}
	}
	
	std::vector<std::vector<bool> > result = findLocalMaxima(cwtVec, scales, 5);
	std::vector<Ridge> ridges = findRidges(result, scales);
	std::vector<size_t> peaks = identifyMajorPeaks(cwtVec, scales, ridges);
	for (size_t k: peaks) {
		std::cout << k << ", ";
	}
}
