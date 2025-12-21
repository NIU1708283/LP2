#pragma once
#include "pch.h"
#include "MapaBase.h"
#include "Util.h"
#include <vector>

// Forward declarations
class GrafSolucio;
class BallTree;

class MapaSolucio : public MapaBase {

public:
    MapaSolucio() {} // Constructor buit
    ~MapaSolucio();  // Destructor net

    void getPdis(std::vector<PuntDeInteresBase *> & pdis);
    void getCamins(std::vector<CamiBase *> & camins);
    void parsejaXmlElements(std::vector<XmlElement> &xmlElements);
    CamiBase * buscaCamiMesCurt(PuntDeInteresBase *desde, PuntDeInteresBase *a);

private:
    std::vector<PuntDeInteresBase *> m_pdis;
    std::vector<CamiBase *> m_camins;

    // ELIMINATS m_graf i m_ballTree per evitar segfaults
    void checkBuit();
};