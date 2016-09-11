#include <cmath>
#include <algorithm>
#include <set>
#include <ccomplex>
#include <cstring>

#include "cwt.h"
#include "convolution/fftwconvolve.h"

std::vector<double> mexh(double scale, size_t maxValue) {
	double coef = 2 / (sqrt(3 * scale) * pow(M_PI, 0.25));
	std::vector<double> vec(maxValue);
	std::iota(vec.begin(), vec.end(), - (maxValue - 1.0) / 2);
	std::transform(vec.begin(), vec.end(), vec.begin(), 
		[&scale, &coef](double x) -> double {
			double val = x * x / (scale * scale);
			double gauss = exp(- val / 2);
			return coef * (1 - val) * gauss; 
		});
	return vec;
}

std::vector<std::vector<double> > cwt(const std::vector<double> &massSpectrum,
									  const std::vector<double> &scales) {
	std::vector<std::vector<double> > answer(scales.size());
	for (size_t i = 0; i < scales.size(); i++) {
		double scale = scales[i];
		auto waveletData = mexh(scale, std::min(size_t(10 * scale), massSpectrum.size()));
		answer[i] = fftwConvolve(massSpectrum, waveletData);
	}
	return answer;
}