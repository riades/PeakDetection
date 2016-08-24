#include "findlocalmaxima.h"
#include <iostream>

std::vector<bool> localMaxima(const std::vector<double> &cwtVec, size_t minWinSize) {
	std::vector<bool> result(cwtVec.size());
	for (int i = 0; i < cwtVec.size() - minWinSize + 1; i++) {
		double maxima = cwtVec[i];
		size_t indMaxima = i;
		for (int j = i + 1; j < i + minWinSize; j++) {
			if (cwtVec[j] > maxima) {
				maxima = cwtVec[j];
				indMaxima = j;
			}
		}
		result[indMaxima] = true;
	}
	return result;
}

std::vector<std::vector<bool> > findLocalMaxima(const std::vector<std::vector<double> > &cwtVec, 
	const std::vector<double> &scales, size_t minWinSize) {
	std::vector<std::vector<bool> > result(scales.size());
	for (size_t i = 0; i < scales.size(); i++) {
		size_t winSize = 2*scales[i] + 1;
		if (winSize < minWinSize) {
			winSize = minWinSize;
		}
		result[i] = localMaxima(cwtVec[i], winSize);
		for (size_t j = 0; j < cwtVec[i].size(); j++) {
			if (cwtVec[i][j] < 0) {
				result[i][j] = false;
			}
		}
	}
	return result;
}
