#pragma once

#include "pch.h" // Caronte
#include "MapaBase.h"
#include "Util.h"
#include <vector>

using namespace std;


// implementacion concreta del mapa
class MapaSolucio : public MapaBase {

public:
	MapaSolucio() {}   // constructor vacio
	~MapaSolucio();    // destructor limpio

	void getPdis(vector<PuntDeInteresBase*>& pdis);

	void getCamins(vector<CamiBase*>& camins);

	// parsea el xml y monta toda la estructura
	void parsejaXmlElements(vector<XmlElement>& xmlElements);

	// new part 2: camino mas corto entre dos pdis
	CamiBase* buscaCamiMesCurt(PuntDeInteresBase* desde, PuntDeInteresBase* a);

	// esta esta bien

private:
	vector<PuntDeInteresBase*> m_pdis;  // puntos de interes
	vector<CamiBase*> m_camins;         // caminos del mapa

	// helpers internos
	void checkBuit(); // limpia vectores antes de recargar
};


/* --------------------------------------------------
 *  lp project - mapa / camins / pdis
 *  arnau baeza muñoz        niu: 1708086
 *  felipe tenorio da silva  niu: 1708283
 * --------------------------------------------------
 */
