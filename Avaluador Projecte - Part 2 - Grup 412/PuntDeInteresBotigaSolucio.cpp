//#include "pch.h"
#include "PuntDeInteresBotigaSolucio.h"
#include <string>

using namespace std;

// Retorna el color del punt d’interès segons el tipus de botiga
unsigned int PuntDeInteresBotigaSolucio::getColor()
{
    // Casos principals segons el tag de la botiga
    if (m_tagBotiga == "supermarket")
        return 0xDFFF00;   // Verd groguenc per supermercat

    if (m_tagBotiga == "tobacco")
        return 0xFF7F50;   // Taronja suau per estanc

    if (m_tagBotiga == "bakery") {
        // Es considera oberta si l’horari conté “06:00” i “22:00”
        const bool has06 = (m_openingHours.find("06:00") != string::npos);
        const bool has22 = (m_openingHours.find("22:00") != string::npos);
        const bool obert = has06 && has22;

        const bool accessible = (m_wheelchair == "yes");

        // Casos de fleca (segons els tests):
        // - Oberta i accessible → verd clar (0x4CBB17)
        // - La resta → salmó (0xFA8072)
        if (accessible && obert)  return 0x4CBB17;
        return 0xFA8072;
    }

    // Resta de botigues: color per defecte segons test (groc)
    return 0xFFEA00;
}

// ---------------------------------------------------------------------------
// Fitxer implementat per Arnau Baeza (NIU 1708086) i Felipe Tenorio da Silva
// ---------------------------------------------------------------------------