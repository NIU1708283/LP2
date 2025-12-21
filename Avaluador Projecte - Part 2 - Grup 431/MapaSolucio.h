#pragma once
#include "MapaBase.h"
#include "Util.h"
#include "PuntDeInteresBase.h"
#include "PuntDeInteresBotigaSolucio.h"
#include "PuntDeInteresRestaurantSolucio.h"
#include "CamiBase.h"
#include "CamiSolucio.h"
using namespace std;

class MapaSolucio :public MapaBase
{
public:
	void getPdis(vector<PuntDeInteresBase*>& pids);

	void getCamins(vector<CamiBase*>& camins);

	void parsejaXmlElements(vector<XmlElement>& xmlElements);

	CamiBase* buscaCamiMesCurt(PuntDeInteresBase* desde, PuntDeInteresBase* a);

	//Destructor
	~MapaSolucio();

private:
	vector<CamiBase*> m_camins; //Lista de caminos
	vector<PuntDeInteresBase*> m_pids; //Lista de puntos de interes
};