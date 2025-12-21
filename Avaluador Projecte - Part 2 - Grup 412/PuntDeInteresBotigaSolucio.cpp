#include "PuntDeInteresBotigaSolucio.h"
#include <string>
#include "pch.h" // Caronte

using namespace std;


// calcula el color segun el tipo de botiga
unsigned int PuntDeInteresBotigaSolucio::getColor()
{
	// supermarket
	if (m_tagBotiga == "supermarket")
		return 0xDFFF00;

	// tobacco
	if (m_tagBotiga == "tobacco")
		return 0xFF7F50;
		
		//dev1

	// bakery (casos especiales)
	if (m_tagBotiga == "bakery") {
		bool obert = (m_openingHours.find("06:00") != string::npos &&
					  m_openingHours.find("22:00") != string::npos);

		bool accessible = (m_wheelchair == "yes"); //dev2

		if (accessible && obert)
			return 0x4CBB17;
			//dev3

		return 0xFA8072;
	}

	// resto de tiendas
	return 0xFFEA00;
}


/* --------------------------------------------------
 *  lp project - mapa / camins / pdis
 *  arnau baeza muñoz        niu: 1708086
 *  felipe tenorio da silva  niu: 1708283
 * --------------------------------------------------
 */
