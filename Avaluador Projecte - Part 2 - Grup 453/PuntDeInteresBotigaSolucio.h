#pragma once

#include "PuntDeInteresBase.h"

class PuntDeInteresBotigaSolucio: public PuntDeInteresBase
{
private:
	std::string m_tag;
    std::string m_hApertura;
	bool m_wChair;

public:
	PuntDeInteresBotigaSolucio(){
		m_tag = "undefinit";
		m_hApertura = "noOpeningHours";
		m_wChair = false;
	}
	PuntDeInteresBotigaSolucio(Coordinate coord, std::string name, std::string tag, std::string apertura, bool chair): PuntDeInteresBase(coord, name), m_tag(tag), m_hApertura(apertura), m_wChair(chair) {}
	~PuntDeInteresBotigaSolucio(){}
	
    unsigned int getColor() override {
        if (m_tag == "supermarket") {
            return 0xA5BE00;
        }
        else if (m_tag == "tobacco") {
            return 0xFFAD69;
        }
        else if (m_tag == "bakery") {
            if (m_hApertura.find("Div 06:00-22:00") != std::string::npos ||
                m_hApertura.find("Dill 06:00-22:00") != std::string::npos) {
                return 0x4CB944;
            }
            else {
                return 0xE85D75;
            }
        }
        return 0xEFD6AC;
    }


    std::string getName() override {
        return PuntDeInteresBase::getName();
    }
};