#include "PuntDeInteresBotigaSolucio.h"

PuntDeInteresBotigaSolucio::PuntDeInteresBotigaSolucio() : m_shop(""), m_wheelchair(""), m_opening_hours("") {} //Constructor per defecte.

PuntDeInteresBotigaSolucio* PuntDeInteresBotigaSolucio::clone() { return new PuntDeInteresBotigaSolucio(*this); } //Clona el punt d'interes.

PuntDeInteresBotigaSolucio::PuntDeInteresBotigaSolucio(PuntDeInteresBotigaSolucio& puntDeInteresBotigaSolucio)
		:PuntDeInteresBase(puntDeInteresBotigaSolucio.getCoord(), puntDeInteresBotigaSolucio.getName()),
		m_shop(puntDeInteresBotigaSolucio.m_shop) {} //Constructor de copia.

PuntDeInteresBotigaSolucio::PuntDeInteresBotigaSolucio(Coordinate coord, std::string name, std::string shop, std::string opening_hours, std::string wheels) : PuntDeInteresBase(coord, name) //Constructor amb parametres.
	{
		m_opening_hours = opening_hours;
		m_wheelchair = wheels;
		m_shop = shop;
	}

string PuntDeInteresBotigaSolucio::getName()  { return PuntDeInteresBase::getName(); } //Retorna el nom del punt d'interes.

void PuntDeInteresBotigaSolucio::setShop(const string shop) { m_shop = shop; } //Estableix el tipus de botiga.
void PuntDeInteresBotigaSolucio::setWheelChair(const string wheelchair) { m_wheelchair = wheelchair; } //Estableix si te acces per a minusvalids.
void PuntDeInteresBotigaSolucio::setOpeningHours(const string opening_hours) { m_opening_hours = opening_hours; } //Estableix l'horari de la botiga.


unsigned int PuntDeInteresBotigaSolucio::getColor()  //Retorna el color del punt d'interes.
{


		if (m_shop == "supermarket")
			return 0xA5BE00;
		if (m_shop == "tobacco")
			return 0xFFAD69;
		if (m_shop == "bakery")
		{
			int pos = m_opening_hours.find("06:00-22:00");
			if (pos != string::npos && m_wheelchair == "yes")
				return 0x4CB944;
			return 0xE85D75;
		}
		return 0xEFD6AC;
}
