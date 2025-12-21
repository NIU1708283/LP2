#pragma once
#include "CamiBase.h"

class CamiSolucio : public CamiBase // Classe derivada de CamiBase
{
public:

	CamiSolucio(); // Constructor per defecte
	CamiSolucio(vector<Coordinate> Cordenades, bool highway); // Constructor paramètric
	CamiSolucio(const CamiSolucio& camiSolucio); // Constructor de copia

	CamiSolucio* clone() override;  // Clon


	///////////Geters
	vector<Coordinate> getCamiCoords() override;  // Retorna les corrdenades
	bool getHighway() override;  // Retorna un true/false


	///////////Seters
	void setCami(vector<Coordinate> Cordenades); 
	void setHighway(bool high); 
	void setCordenada(Coordinate cord); 

	///////////Destructor
	~CamiSolucio() { m_Cordenades.clear();}
	

private:
	vector<Coordinate> m_Cordenades;
	bool m_highway;
};