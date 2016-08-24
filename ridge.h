#ifndef RIDGE_H_INCLUDED
#define RIDGE_H_INCLUDED

#include <vector>
#include <cstdlib>

struct Ridge {	
	std::vector<size_t> positions;
	int gapNumber = 0;
	size_t level = 0;
};

#endif