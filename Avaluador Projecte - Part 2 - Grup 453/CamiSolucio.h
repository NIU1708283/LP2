#pragma once
#include "CamiBase.h"
#include "Common.h"
#include <vector>
#include <ostream>

class CamiSolucio : public CamiBase {
private:
    std::vector<Coordinate> m_coords;
    std::string m_nom;
public:
    CamiSolucio(){}
    CamiSolucio(std::vector<Coordinate> coord, std::string name){
        m_coords = coord;
        m_nom = name;
    }
    ~CamiSolucio(){
    }
    std::vector<Coordinate> getCamiCoords(){
        return m_coords;
    }
};
