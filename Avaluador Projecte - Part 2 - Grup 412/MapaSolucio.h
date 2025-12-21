#pragma once
#include "pch.h"
#include "MapaBase.h"
#include "GrafSolucio.h"
#include "BallTree.h"
#include "Util.h"

class MapaSolucio : public MapaBase {

public:
    MapaSolucio() {
        m_graf = nullptr;
        m_ballTree = nullptr;
    }
    
    ~MapaSolucio();

    void getPdis(std::vector<PuntDeInteresBase *> & pdis);
    void getCamins(std::vector<CamiBase *> & camins);
    void parsejaXmlElements(std::vector<XmlElement> &xmlElements);
    CamiBase * buscaCamiMesCurt(PuntDeInteresBase *desde, PuntDeInteresBase *a);

private:
    std::vector<PuntDeInteresBase *> m_pdis;
    std::vector<CamiBase *> m_camins;
    GrafSolucio* m_graf;
    BallTree* m_ballTree;

    void checkBuit();
};