#ifndef FINDRIDGE_H_INCLUDED
#define FINDRIDGE_H_INCLUDED

#include "ridge.h"

std::vector<Ridge> findRidges(const std::vector<std::vector<bool> > &localMax, 
							  const std::vector<double> &scales, 
							  size_t minWinSize = 5, size_t gapTh = 3);

#endif