#ifndef CWT_H_INCLUDED
#define CWT_H_INCLUDED

#include <vector>
#include <cstdlib>
#include <functional>

using waveletFunction = std::function<double(double, double)>;

double mexh(double x, double scale);

class CWT {
private:
	const size_t lengthOut = 1024;
	const double maxValue = 8;
	const double step;
	waveletFunction wavelet;
	std::vector<double> psi;
public:
	CWT(waveletFunction _wavelet = mexh);
	std::vector<std::vector<double> > compute(const std::vector<double> &massSpectrum, 
											  const std::vector<double> &scales) const;
};

#endif