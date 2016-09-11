#ifndef CWT_H_INCLUDED
#define CWT_H_INCLUDED

#include <vector>
#include <cstdlib>

std::vector<std::vector<double> > cwt(const std::vector<double> &massSpectrum,
                                     const std::vector<double> &scales);
#endif