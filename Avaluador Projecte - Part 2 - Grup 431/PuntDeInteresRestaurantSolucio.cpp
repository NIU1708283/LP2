
#include "PuntDeInteresRestaurantSolucio.h"


PuntDeInteresRestaurantSolucio::PuntDeInteresRestaurantSolucio() : m_cuisine(""), m_wheelchair("") { } // Constructor per defecte

PuntDeInteresRestaurantSolucio::PuntDeInteresRestaurantSolucio(Coordinate coord, string name, string cuisine, string wheels) :
		PuntDeInteresBase(coord, name), m_cuisine(cuisine), m_wheelchair(wheels) {} // Constructor paramètric

PuntDeInteresRestaurantSolucio::PuntDeInteresRestaurantSolucio(PuntDeInteresRestaurantSolucio& puntDeInteresRestaurantSolucio)
		:PuntDeInteresBase(puntDeInteresRestaurantSolucio.getCoord(), puntDeInteresRestaurantSolucio.getName()),
		m_cuisine(puntDeInteresRestaurantSolucio.m_cuisine),
		m_wheelchair(puntDeInteresRestaurantSolucio.m_wheelchair) {} // Constructor de copia

PuntDeInteresRestaurantSolucio* PuntDeInteresRestaurantSolucio::clone() { return new PuntDeInteresRestaurantSolucio(*this); } // Clon

void PuntDeInteresRestaurantSolucio::setCuisine(const string cuisine) { m_cuisine = cuisine; }
void PuntDeInteresRestaurantSolucio::setWheelChair(const string wc) { m_wheelchair = wc; }

string PuntDeInteresRestaurantSolucio::getName() { return PuntDeInteresBase::getName(); }

unsigned int PuntDeInteresRestaurantSolucio::getColor() 
{
		unsigned int color;

		if ((m_wheelchair == "yes") && (m_cuisine != "")) //&&(m_cuisine!="")
		{
			if (m_cuisine == "pizza")
				return 0x03FCBA;
			return 0x251351;
		}
		if (m_cuisine == "chinese")
			return 0xA6D9F7;
		return PuntDeInteresBase::getColor();

		return color;
}
