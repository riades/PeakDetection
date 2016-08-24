#ifndef CWT_H_INCLUDED
#define CWT_H_INCLUDED

#include <vector>
#include <cstdlib>

typedef std::vector<double> (*waveletFunction)(size_t, double, double);

std::vector<double> mexh(size_t lengthOut, double scale, double maxvalue);
std::vector<std::vector<double> > cwt(const std::vector<double> &massSpectrum, 
	const std::vector<double> &scales, waveletFunction wavelet);

#endif