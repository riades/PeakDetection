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

int main() {
	std::string line;
  	std::vector<double> vec;
  	std::ifstream test_file ("data/test.txt");
  	if (!test_file.is_open()) {
    	return 0;
  	}
  	while (getline(test_file,line)) {
    	std::stringstream stream(line);
    	double mass, intensity;
    	stream >> mass >> intensity;
    	vec.push_back(intensity);
  	} 
  	test_file.close();
	std::vector<double> scales(63);
	std::iota(std::begin(scales), std::end(scales), 1);
	std::vector<std::vector<double> > cwtVec = cwt(vec, scales, &mexh);
	std::ifstream wcoefs_file("data/wcoefs.txt");
	for (size_t i = 0; i < cwtVec.size(); i++) {
		getline(wcoefs_file,line);
		std::stringstream stream(line);
		for (size_t j = 0; j < cwtVec[i].size(); j++) {
			double val;
			stream >> val;
			cwtVec[i][j] = val;
		}
	}
	
	std::vector<std::vector<bool> > result = findLocalMaxima(cwtVec, scales, 5);
	std::ifstream localmax_file("data/localmax.txt");
	for (size_t i = 0; i < result.size(); i++) {
		getline(localmax_file,line);
		std::stringstream stream(line);
		for (size_t j = 0; j < result[i].size(); j++) {
			int val;
			stream >> val;
			result[i][j] = val;
		}
	}
	
	std::vector<Ridge> ridges = findRidges(result, scales);
	std::vector<size_t> peaks = identifyMajorPeaks(cwtVec, scales, ridges);
	for (size_t k: peaks) {
		std::cout << k << ", ";
	}
}
