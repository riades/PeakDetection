#include <cmath>
#include <algorithm>
#include <set>

#include "cwt.h"
#include "convolution/fftwconvolve.h"

double mexh(double x, double scale) {
	double coef = 2 / (pow(3.0 * scale, 0.5) * pow(M_PI, 0.25));
	double val = x * x / (scale * scale);
	double gauss = exp(- val / 2);
	return coef * (1 - val) * gauss;
}

CWT::CWT(waveletFunction _wavelet) : wavelet(_wavelet), 
									 psi(lengthOut), 
									 step(2 * maxValue/(lengthOut - 1)) {
    psi[0] = -maxValue;
    double step = 2 * maxValue/(lengthOut - 1);
	for (size_t i = 1; i < psi.size(); i++) {
		psi[i] = psi[i - 1] + step;
	}
	std::transform(psi.begin(), psi.end(), psi.begin(), 
		[](double x) -> double {
			return mexh(x, 1);
		});
}

std::vector<std::vector<double> > CWT::compute(const std::vector<double> &massSpectrum, 
											   const std::vector<double> &scales) const {
	std::vector<std::vector<double> > answer;
	answer.reserve(scales.size());
	for (double scale: scales) {
		std::set<size_t> selected;
		for (int i = 0; i <= 2 * scale * maxValue; i++) {
			selected.insert(i / (scale * step));
		} 
		size_t waveLength = selected.size();
		std::vector<double> waveletData;
		waveletData.reserve(massSpectrum.size());
		double sum = 0;
		for (size_t j: selected) {
			sum += psi[j];
		}
		for (size_t j: selected) {
			waveletData.push_back(psi[j] - sum/selected.size());
		}
		waveletData.resize(massSpectrum.size());
		std::vector<double> res = fftwConvolve(massSpectrum, waveletData);
		std::transform(res.begin(), res.end(), res.begin(),
			[&scale](double x) -> double {
				return x / sqrt(scale);
			});
		std::vector<double> cwtCoefs;
		size_t shift = massSpectrum.size() - waveLength/2;
		cwtCoefs.insert(cwtCoefs.end(), res.begin() + shift, res.end());
		cwtCoefs.insert(cwtCoefs.end(), res.begin(), res.begin() + shift);
		answer.push_back(cwtCoefs);
	}
	return answer; 
}