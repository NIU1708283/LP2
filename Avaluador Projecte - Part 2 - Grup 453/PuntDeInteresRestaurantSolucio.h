#pragma once

#include "PuntDeInteresBase.h"

class PuntDeInteresRestaurantSolucio: public PuntDeInteresBase
{
private:
	bool m_wChair;
	std::string m_tipusCuina;
public:
	PuntDeInteresRestaurantSolucio(Coordinate coord, std::string name, std::string cuina, bool chair): PuntDeInteresBase(coord, name), m_tipusCuina(cuina), m_wChair(chair) {}
    PuntDeInteresRestaurantSolucio(){
        m_wChair = false;
        m_tipusCuina = "";
    }
    ~PuntDeInteresRestaurantSolucio(){}
	
    std::string getName() {
        return PuntDeInteresBase::getName();
    }

    unsigned int getColor() {
        if (m_tipusCuina == "pizza" && m_wChair){
            return 0x03FCBA;
        }else if (m_tipusCuina == "chinese"){
            return 0xA6D9F7;
        }else if (m_wChair){
            return 0x251351;
        }
        return PuntDeInteresBase::getColor();
    }
};