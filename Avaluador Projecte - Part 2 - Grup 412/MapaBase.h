#pragma once

#include "pch.h" // Caronte
#include "Common.h"
#include "PuntDeInteresBase.h"
#include "CamiBase.h"
#include <vector>

using namespace std;


// interfaz base del mapa
class MapaBase {
public:
	// devuelve todos los puntos de interes
	virtual void getPdis(vector<PuntDeInteresBase*>& pdis) = 0;

	// devuelve todos los caminos
	virtual void getCamins(vector<CamiBase*>& camins) = 0;

	// parsea el xml y construye el mapa
	virtual void parsejaXmlElements(vector<XmlElement>& xmlElements) = 0;

	// new part 2: buscar el camino mas corto entre dos pdis
	virtual CamiBase* buscaCamiMesCurt(PuntDeInteresBase* desde, PuntDeInteresBase* a) = 0;

};


/* --------------------------------------------------
 *  lp project - mapa / camins / pdis
 *  arnau baeza muñoz        niu: 1708086
 *  felipe tenorio da silva  niu: 1708283
 * --------------------------------------------------
 */
