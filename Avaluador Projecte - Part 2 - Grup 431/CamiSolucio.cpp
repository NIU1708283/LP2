#include "CamiSolucio.h"


	CamiSolucio::CamiSolucio() { m_highway = false; } // Constructor per defecte

	CamiSolucio::CamiSolucio(vector<Coordinate> Cordenades, bool highway) // Constructor paramètric
	{
		m_Cordenades = Cordenades;
		m_highway = highway;
	}

	CamiSolucio::CamiSolucio(const CamiSolucio& camiSolucio) // Constructor de copia
	{
		m_highway = camiSolucio.m_highway;
		m_Cordenades = camiSolucio.m_Cordenades;
	}

	/*
	CamiSolucio()
	{ //TASCA 4
		m_Cordenades.push_back(Coordinate{ 41.4928803, 2.1452381 });
		m_Cordenades.push_back(Coordinate{41.4929072, 2.1452474});
		m_Cordenades.push_back(Coordinate{41.4933070, 2.1453852});
		m_Cordenades.push_back(Coordinate{ 41.4939882, 2.1456419 });
	}
	*/

	CamiSolucio* CamiSolucio::clone() { return new CamiSolucio(*this); } // Clon

	vector<Coordinate> CamiSolucio::getCamiCoords()  { return m_Cordenades; } // Retorna les corrdenades
	bool CamiSolucio::getHighway()  { return m_highway; } // Retorna un true/false

	void CamiSolucio::setCami(vector<Coordinate> Cordenades) { m_Cordenades = Cordenades; }
	void CamiSolucio::setHighway(bool high) { m_highway = high; }
	void CamiSolucio::setCordenada(Coordinate cord) { m_Cordenades.push_back(cord); }


