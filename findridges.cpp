#include "findridges.h"
#include <algorithm>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

std::set<size_t> whichTrue(std::vector<bool> vec) {
	std::set<size_t> result;
	for (size_t i = 0; i < vec.size(); i++) {
		if (vec[i]) {
			result.insert(i);
		}
	}
	return result;
}

std::vector<Ridge> findRidges(const std::vector<std::vector<bool> > &localMax, 
	const std::vector<double> &scales, size_t minWinSize, size_t gapTh) {

	size_t nMz = localMax[0].size();
	std::unordered_map<size_t, Ridge> ridgeHashTable;
	std::vector<Ridge> resultRidgeVec;
	std::map<size_t, size_t> currMaxMap;
	// Initialize the ridge lines based on the local maxima points 
	// identified at the largest scale
	std::set<size_t> currMaxSet = whichTrue(localMax.back());
	size_t ridgeCounter = 0;
	for (size_t ind: currMaxSet) {
		Ridge ridge;
		ridge.positions.push_back(ind);
		ridgeHashTable[ridgeCounter] = std::move(ridge);
		currMaxMap[ind] = ridgeCounter;
		ridgeCounter++;
	}
	for (int i = scales.size() - 2; i >= 0; i--) {
		if (currMaxSet.size() == 0) {
			currMaxSet = whichTrue(localMax[i]);
			for (size_t ind: currMaxSet) {
				Ridge ridge;
				ridge.positions.push_back(ind);
				if (currMaxMap.find(ind) != currMaxMap.end()) { 
					ridge.level = i;
					resultRidgeVec.push_back(ridge);
				} else {
					ridgeHashTable[ridgeCounter] = std::move(ridge);
					currMaxMap[ind]  = ridgeCounter;
					ridgeCounter++;
				}
			}
			continue;
		}
		// The maximum allowed distance between the nearest points should be
		// less than the sliding window size at that scale level.
		size_t winSize = 2*scales[i] + 1;
		if (winSize < minWinSize) {
			winSize = minWinSize;
		}
		std::unordered_set<size_t> removed;
		std::set<size_t> nextMaxSet; 
		for (size_t ind: currMaxSet) {
			size_t ridgeName = currMaxMap[ind];
			int status = ridgeHashTable[ridgeName].gapNumber;
			size_t prevMaxInd = ridgeHashTable[ridgeName].positions.back();
			// For each ridge line with its gap number less than a certain threshold,
			// search the nearest maximum point at the next adjacent scale
			if (status > gapTh && scales[i] >= 2) {
				std::vector<size_t> tempPos = ridgeHashTable[ridgeName].positions;
				Ridge ridge;
				ridge.positions.assign(tempPos.begin(), tempPos.end() - status);
				ridge.level = i;
				resultRidgeVec.push_back(ridge);
				removed.insert(ridgeName);
				currMaxMap.erase(prevMaxInd);
				continue;
			}
			size_t newMaxInd;
			size_t startInd = (ind > winSize) ? ind - winSize : 0;
			size_t endInd = (ind + winSize < nMz - 1) ? ind + winSize : nMz - 1;
			std::set<size_t> temp = whichTrue(
				std::vector<bool> (localMax[i].begin() + startInd, localMax[i].begin() + endInd)
				);
			// If there is no closest point found, the gap number of the ridge line
			// is increased by one, or else the gap number is set to zero.
			if (temp.size() == 0) {
				ridgeHashTable[ridgeName].gapNumber++;
				newMaxInd = ind;
			} else {
				ridgeHashTable[ridgeName].gapNumber = 0;
				newMaxInd = *temp.begin() + startInd;
				for (size_t t: temp) {
					t += startInd;
					if (abs(t - ind) < abs(newMaxInd - ind)) {
						newMaxInd = t;
					}
				}
			}
			ridgeHashTable[ridgeName].positions.push_back(newMaxInd);
			nextMaxSet.insert(newMaxInd);
			if (newMaxInd != prevMaxInd) {
				currMaxMap.erase(prevMaxInd);
			}
			if (currMaxMap.find(newMaxInd) != currMaxMap.end()) {
				size_t oldRidgeName = currMaxMap[newMaxInd];
				if (ridgeName != oldRidgeName) {
					size_t oldSize = ridgeHashTable[oldRidgeName].positions.size();
					size_t currSize = ridgeHashTable[ridgeName].positions.size();
					size_t rmName;
					if (oldSize > currSize) {
						rmName = ridgeName;
					} else {	
						currMaxMap[newMaxInd] = ridgeName;
						rmName = oldRidgeName;
					}
					std::vector<size_t> tempPos = ridgeHashTable[rmName].positions;
					int status = ridgeHashTable[rmName].gapNumber;
					Ridge ridge;
					ridge.positions.assign(tempPos.begin(), tempPos.end() - status);
					ridge.level = i;
					resultRidgeVec.push_back(ridge);
					removed.insert(rmName);
				}	
			} else {
				currMaxMap[newMaxInd] = ridgeName;
			}
		}
		for (size_t ind: removed) {
			ridgeHashTable.erase(ind);
		}
		std::set<size_t> unselectedMaxInd = whichTrue(localMax[i]);
		std::set<size_t> selectedMaxInd;
		std::set_difference(unselectedMaxInd.begin(), unselectedMaxInd.end(), 
			nextMaxSet.begin(), nextMaxSet.end(), 
			std::inserter(selectedMaxInd, selectedMaxInd.end()));	
		for (size_t ind: selectedMaxInd) {
			Ridge ridge;
			ridge.positions.push_back(ind);
			if (currMaxMap.find(ind) != currMaxMap.end()) { 
				ridge.level = i;
				resultRidgeVec.push_back(ridge);
			} else {
				nextMaxSet.insert(ind);
				ridgeHashTable[ridgeCounter] = std::move(ridge);
				currMaxMap[ind] = ridgeCounter;
				ridgeCounter++;
			}
		}
		currMaxSet = nextMaxSet;
	}
	//TODO: delete duplicated ridges
	for (auto it = ridgeHashTable.begin(); it != ridgeHashTable.end(); ++it) {
		resultRidgeVec.push_back(it->second);
	}
	/*std::sort(resultRidgeVec.begin(), resultRidgeVec.end(), 
		[](const Ridge a, const Ridge b) -> bool {
			return a.positions.back() < b.positions.back();
		});
	*/
	return resultRidgeVec;
}