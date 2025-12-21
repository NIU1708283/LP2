//#include "pch.h"
#include "PuntDeInteresRestaurantSolucio.h"
#include <string>

using namespace std;

// Retorna el color del restaurant segons el tipus i accessibilitat
unsigned int PuntDeInteresRestaurantSolucio::getColor()
{
    // Restaurants de pizza accessibles → color verd clar
    if (m_tipusCuisine == "pizza" && m_wheelchair == "yes")
        return 0x7FFFD4;

    // Restaurants xinesos → color cian
    if (m_tipusCuisine == "chinese")
        return 0x00FFFF;

    // Qualsevol altre restaurant amb accessibilitat → violeta fosc
    if (m_wheelchair == "yes" && !m_tipusCuisine.empty())
        return 0x5D3FD3;

    // Per defecte (color base dels punts d’interès)
    return PuntDeInteresBase::getColor(); // Esperat: 0xFFA500
}

// ---------------------------------------------------------------------------
// Fitxer implementat per Arnau Baeza (NIU 1708086) i Felipe Tenorio da Silva
// ---------------------------------------------------------------------------