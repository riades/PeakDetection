#ifndef FINDLOCALMAXIMA_H_INCLUDED
#define FINDLOCALMAXIMA_H_INCLUDED

#include <vector>
#include <cstdlib>

std::vector<std::vector<bool> > findLocalMaxima(const std::vector<std::vector<double> > &cwtVec, 
	const std::vector<double> &scales, size_t minWinSize);

#endif