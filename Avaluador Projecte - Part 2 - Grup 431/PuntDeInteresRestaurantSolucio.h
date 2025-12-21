#pragma once
#include "PuntDeInteresBase.h"
using namespace std;

class PuntDeInteresRestaurantSolucio : public PuntDeInteresBase // Classe derivada de PuntDeInteresBase
{
public:
	PuntDeInteresRestaurantSolucio(); // Constructor per defecte

	PuntDeInteresRestaurantSolucio(Coordinate coord, string name, string cuisine, string wheels); // Constructor paramètric

	PuntDeInteresRestaurantSolucio(PuntDeInteresRestaurantSolucio& puntDeInteresRestaurantSolucio); // Constructor de copia

	PuntDeInteresRestaurantSolucio* clone();  // Clon

	/////////Seters
	void setCuisine(const string cuisine); 
	void setWheelChair(const string wc); 

	/////////Geters
	string getName() override; 
	unsigned int getColor() override;


private:
	string m_cuisine;
	string m_wheelchair;
};