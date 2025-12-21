#pragma once

#include "pch.h" // Caronte
#include "Common.h"
#include <vector>

using namespace std;

class CamiBase { // clase base para cualquier camino
public:
	virtual vector<Coordinate> getCamiCoords() = 0; // devuelve las coordenadas del camino
};


/* --------------------------------------------------
 *  lp project - mapa / camins / pdis
 *  arnau baeza muñoz        niu: 1708086
 *  felipe tenorio da silva  niu: 1708283
 * --------------------------------------------------
 */
