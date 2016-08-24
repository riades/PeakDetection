#include "identifymajorpeaks.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <limits>

std::vector<size_t> identifyMajorPeaks(const std::vector<std::vector<double> > &cwtVec, 
	const std::vector<double> &scales, const std::vector<Ridge> &ridgeVec, 
	double minScale, double maxScale, double SNRTh, size_t ridgeLenTh, 
	size_t noiseWinSize, double minNoiseLevel) {

	size_t nMz = cwtVec[0].size();
	std::vector<size_t> mzInd(ridgeVec.size());
	for (size_t i = 0; i < ridgeVec.size(); i++) {
		mzInd[i] = ridgeVec[i].positions.back();
	}
	std::vector<double> peakVal(ridgeVec.size());
	for (size_t i = 0; i < ridgeVec.size(); i++) {
		double value = 0;
		for (size_t j = 0; j < ridgeVec[i].positions.size(); j++) {
			size_t scale = scales[j + ridgeVec[i].level];
			size_t k = ridgeVec[i].positions[ridgeVec[i].positions.size() - 1 - j];
			if (scale <= maxScale && scale >= minScale) {
				if (cwtVec[j][k] > value) {
					value = cwtVec[j][k];
				}
			}
		} 
		peakVal[i] = value;
	}
	std::vector<double> noise = cwtVec[0];
	std::transform(noise.begin(), noise.end(), noise.begin(), 
		[](double x) -> double {
			return x > 0 ? x : -x; 
		});
	std::vector<double> peakSNR(ridgeVec.size());
	for (size_t i = 0; i < ridgeVec.size(); i++) {
		size_t ind = mzInd[i];
		int startInd = (ind > noiseWinSize) ? ind - noiseWinSize : 0;
		int endInd = (ind + noiseWinSize < nMz - 1) ? ind + noiseWinSize : nMz - 1;
		double q = round((endInd - startInd + 1) * 0.95);
		std::vector<double> temp;
		temp.assign(noise.begin() + startInd, noise.begin() + endInd);
		std::nth_element(temp.begin(), temp.begin() + q, temp.end());
		double noiseLevel = temp[q];
		if (noiseLevel < minNoiseLevel) {
			noiseLevel = minNoiseLevel;
		}
		peakSNR[i] = peakVal[i]/noiseLevel;
	}
	std::vector<size_t> selected;
	for (size_t i = 0; i < ridgeVec.size(); i++) {
		if (scales[ridgeVec[i].level + ridgeVec[i].positions.size() - 1] >= ridgeLenTh 
			&& peakSNR[i] > SNRTh) {
			selected.push_back(mzInd[i]);
		}
	}
	std::sort(selected.begin(), selected.end());
	return selected;
}