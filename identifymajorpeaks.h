#ifndef IDENTIFYPEAKS_H_INCLUDED
#define IDENTIFYPEAKS_H_INCLUDED

#include <limits>

#include "ridge.h"
std::vector<size_t> identifyMajorPeaks(const std::vector<std::vector<double> > &cwtVec, 
									   const std::vector<double> &scales, 
									   const std::vector<Ridge> &ridgeVec, 
									   double minScale = 5, 
									   double maxScale = std::numeric_limits<double>::max(), 
									   double SNRTh = 3, size_t ridgeLenTh = 32, 
									   size_t noiseWinSize = 500, double minNoiseLevel = 0.001);

#endif