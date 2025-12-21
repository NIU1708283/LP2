// NEW PART 2
#pragma once
#include "pch.h"
#include "MapaBase.h"
#include "GrafSolucio.h"
#include "BallTree.h"
#include "CamiSolucio.h" // Suposant que tenim aquest fitxer de la Part 1

class MapaSolucio : public MapaBase {

public:
    MapaSolucio() {
        m_graf = new GrafSolucio();
        m_ballTree = nullptr;
    }

    ~MapaSolucio() {
        if (m_graf) delete m_graf;
        if (m_ballTree) delete m_ballTree;
        for (auto p : m_pdis) delete p;
        for (auto c : m_camins) delete c;
    }

    // Metodes a implementar de la primera part
    void getPdis(std::vector<PuntDeInteresBase*>& pdis);
    void getCamins(std::vector<CamiBase*>& camins);
    void parsejaXmlElements(std::vector<XmlElement>& xmlElements);

    // Metode a implementar de la segona part
    CamiBase* buscaCamiMesCurt(PuntDeInteresBase* desde, PuntDeInteresBase* a);

private:
    std::vector<PuntDeInteresBase*> m_pdis;
    std::vector<CamiBase*> m_camins;
    void checkBuit();
    
    // Noves estructures Part 2
    GrafSolucio* m_graf;
    BallTree* m_ballTree;
};