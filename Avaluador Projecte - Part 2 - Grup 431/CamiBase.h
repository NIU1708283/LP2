#ifndef _CAMI_BASE_H
#define _CAMI_BASE_H

#include "Common.h"
#include <vector>
#include <iostream>
using namespace std;

class CamiBase {
public:
	virtual std::vector<Coordinate> getCamiCoords() = 0;
	virtual CamiBase* clone() = 0;
	virtual bool getHighway() = 0;
};

#endif
