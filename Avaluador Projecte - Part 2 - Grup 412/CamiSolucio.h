#pragma once
#include "CamiBase.h"

class CamiSolucio : public CamiBase
{
public:
    CamiSolucio() { valor_hw = false; }
    // Constructor que accepta vector de coordenades (necessari per a MapaSolucio)
    CamiSolucio(std::vector<Coordinate> coords) { m_coords = coords; valor_hw = false; }
    CamiSolucio(std::vector<Coordinate> coords, bool hw) { m_coords = coords; valor_hw = hw; }
    CamiSolucio(const CamiSolucio& cami) { m_coords = cami.m_coords; valor_hw = cami.valor_hw; }
    ~CamiSolucio() {}

    void SetCami(std::vector<Coordinate> coords) { m_coords = coords; }
    void setHw(bool hw) { valor_hw = hw; }
    void setCoord(Coordinate c) { m_coords.push_back(c); }

    // Eliminem 'override' perquè no són a CamiBase
    bool esHighway() { return valor_hw; }
    
    // Implementació obligatòria de la classe base
    std::vector<Coordinate> getCamiCoords() { return m_coords; }

    CamiSolucio* clone() { return new CamiSolucio(*this); }

private:
    std::vector<Coordinate> m_coords;
    bool valor_hw;
};