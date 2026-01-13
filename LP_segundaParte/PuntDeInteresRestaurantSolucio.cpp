#include "PuntDeInteresRestaurantSolucio.h"
#include <string>
#include "pch.h" // Caronte

using namespace std;


// calcula el color segun tipo de cocina y accesibilidad
unsigned int PuntDeInteresRestaurantSolucio::getColor()
{
	// pizza y accesible
	if (m_tipusCuisine == "pizza" && m_wheelchair == "yes")
		return 0x7FFFD4;

	// chinoooo
	if (m_tipusCuisine == "chinese")
		return 0x00FFFF;

	// accesible con tipo definido
	if (m_wheelchair == "yes" && !m_tipusCuisine.empty())
		return 0x5D3FD3;

	// por defecto
	return PuntDeInteresBase::getColor();
}


/* --------------------------------------------------
 *  lp project - mapa / camins / pdis
 *  arnau baeza muñoz        niu: 1708086
 *  felipe tenorio da silva  niu: 1708283
 * --------------------------------------------------
 */
