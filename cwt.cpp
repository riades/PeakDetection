#include "cwt.h"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <iostream>

std::vector<double> mexh(size_t lengthOut, double scale, double maxvalue) {
	maxvalue *= scale;
	double coef = 2 / (pow(3.0 * scale, 0.5) * pow(M_PI, 0.25));
    std::vector<double> vec(lengthOut);
    vec[0] = -maxvalue;

    double step = 2*maxvalue/(lengthOut - 1);
	for (size_t i = 1; i < vec.size(); i++) {
		vec[i] = vec[i - 1] + step;
	}
	std::transform(vec.begin(), vec.end(), vec.begin(), 
		[&scale, &coef](double x) -> double {
			double val = x * x / (scale * scale);
			double gauss = exp(- val / 2);
			return coef * (1 - val) * gauss; 
		});
	double sum = 0;
	for (size_t i = 0; i < vec.size(); i++) {
		sum += vec[i];
	}
	return vec;
}

std::vector<double> convolve(const std::vector<double> &volume, const std::vector<double> &kernel) {
	/*size_t volumeSize = volume.size(); 
	size_t kernelSize = kernel.size();
	size_t resultSize = std::max(kernelSize, volumeSize); 
	std::vector<double> result(resultSize);
    for (int i = 0; i < resultSize; i++) {
        for (int j = 0; j < resultSize; j++) {
        	int k = (i - j) % resultSize;
        	if (j < volumeSize && k < kernelSize) {
        		result[i] += volume[j] * kernel[k];
        	}
        }
    }
    return result;
    */
    size_t volumeSize = volume.size(); 
	size_t kernelSize = kernel.size(); 
    size_t resultSize = volumeSize + kernelSize - 1;
	std::vector<double> result(resultSize);
    for (size_t i = 0; i < volumeSize; i++) {
        for (size_t j = 0; j < kernelSize; j++) {
        	result[i + j] += volume[i] * kernel[j];
        }
    }
    auto first = result.begin() + (kernelSize - 1)/2;
	auto last = result.begin() + volumeSize + (kernelSize - 1)/2;
	return std::vector<double>(first, last);
}

std::vector<std::vector<double> > cwt(const std::vector<double> &massSpectrum, 
	const std::vector<double> &scales, waveletFunction wavelet) {
	std::vector<std::vector<double> > answer;
	answer.reserve(scales.size());
	for (double scale: scales) {
		std::vector<double> waveletData = wavelet(massSpectrum.size(), scale, 10);
		answer.push_back(convolve(massSpectrum, waveletData));
	}
	return answer; 
}
