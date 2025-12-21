#pragma once

#include "PuntDeInteresBase.h"
using namespace std;

class PuntDeInteresBotigaSolucio : public PuntDeInteresBase //Classe filla de PuntDeInteresBase.
{
public:
	PuntDeInteresBotigaSolucio(); //Constructor per defecte.

	PuntDeInteresBotigaSolucio* clone(); //Clona el punt d'interes.

	PuntDeInteresBotigaSolucio(PuntDeInteresBotigaSolucio& puntDeInteresBotigaSolucio); //Constructor de copia.

	PuntDeInteresBotigaSolucio(Coordinate coord, std::string name, std::string shop, std::string opening_hours, std::string wheels); //Constructor amb parametres.

	/////////Geters
	string getName() override;  //Retorna el nom del punt d'interes.
	


	/////////Seters
	void setWheelChair(const string wheelchair);  //Estableix si te acces per a minusvalids.
	void setOpeningHours(const string opening_hours);  //Estableix l'horari de la botiga.
	void setShop(const string shop);

	unsigned int getColor() override; //Retorna el color del punt d'interes.

private:
	string m_opening_hours;
	string m_wheelchair;
	string m_shop;
};