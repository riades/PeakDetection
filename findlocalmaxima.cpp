#include "findlocalmaxima.h"
#include <iostream>

std::vector<bool> localMaxima(const std::vector<double> &cwtVec, size_t winSize) {
	std::vector<bool> result(cwtVec.size());
	size_t halfWinSize = winSize / 2;
	size_t i = 0;
	size_t prevIndMax;
	while (i < cwtVec.size()) {
		size_t last;
		if (i % 2 == 0) {
			last = i + winSize - halfWinSize;
		} else {
			last = i + halfWinSize;
		}
		if (last > cwtVec.size()) {
			last = cwtVec.size();
		}
		double maxVal = cwtVec[i];
		size_t indMax = i;
		for (size_t j = i + 1; j < last; j++) {
			if (cwtVec[j] > maxVal) {
				maxVal = cwtVec[j];
				indMax = j;
			}
		}
		if (i == 0 || i != 0 && indMax - prevIndMax >= winSize) {
			result[indMax] = true;	
			prevIndMax = indMax;
		} else {
			if (cwtVec[prevIndMax] < cwtVec[indMax]) {
				result[indMax] = true;	
				result[prevIndMax] = false;
				prevIndMax = indMax;
			}
		}
		i = last;
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
