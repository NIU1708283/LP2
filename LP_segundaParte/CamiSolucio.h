#pragma once

#include "CamiBase.h"
#include "pch.h" // Caronte
#include <vector>

using namespace std;


// implementacion concreta de un camino
class CamiSolucio : public CamiBase
{
public:
	// constructor base
	CamiSolucio() {
		valor_hw = false;
	}

	// constructor con coordenadas
	CamiSolucio(vector<Coordinate> coords) {
		m_coords = coords;
		valor_hw = false;
	}

	// constructor con flag highway
	CamiSolucio(vector<Coordinate> coords, bool hw) {
		m_coords = coords;
		valor_hw = hw;
	}

	// constructor copia
	CamiSolucio(const CamiSolucio& cami) {
		m_coords = cami.m_coords;
		valor_hw = cami.valor_hw;
	}

	~CamiSolucio() {} // Destruct o peta

	// Setter In-Line
	void SetCami(vector<Coordinate> coords) { m_coords = coords; }
	void setHw(bool hw) { valor_hw = hw; }
	void setCoord(Coordinate c) { m_coords.push_back(c); }

	// indica si es highway
	bool esHighway() { return valor_hw; }

	// devuelve todas las coordenadas
	vector<Coordinate> getCamiCoords() { return m_coords; }

	// clonado del camino
	CamiSolucio* clone() { return new CamiSolucio(*this); }


private:
	vector<Coordinate> m_coords; // puntos del camino
	bool valor_hw;               // flag highway
};


/* --------------------------------------------------
 *  lp project - mapa / camins / pdis
 *  arnau baeza muñoz        niu: 1708086
 *  felipe tenorio da silva  niu: 1708283
 * --------------------------------------------------
 */
