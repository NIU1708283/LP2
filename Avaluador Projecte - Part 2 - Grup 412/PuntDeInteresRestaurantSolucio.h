#pragma once

#include <string>
#include "Common.h"
#include "PuntDeInteresBase.h"
#include "pch.h" // Caronte

using namespace std;

// Classe que representa un restaurant amb informació addicional
class PuntDeInteresRestaurantSolucio : public PuntDeInteresBase
{
public:
    // Constructors i destructor
    PuntDeInteresRestaurantSolucio() : m_tipusCuisine(""), m_wheelchair("") {}

    PuntDeInteresRestaurantSolucio(Coordinate c, string nom, string cuisine, string rodes)
        : PuntDeInteresBase(c, nom), m_tipusCuisine(cuisine), m_wheelchair(rodes) {}

    // Constructor còpia
    PuntDeInteresRestaurantSolucio(PuntDeInteresRestaurantSolucio& altre)
        : PuntDeInteresBase(altre.getCoord(), altre.getName()),
          m_tipusCuisine(altre.m_tipusCuisine),
          m_wheelchair(altre.m_wheelchair) {}

    ~PuntDeInteresRestaurantSolucio() {}

    // Getters
    unsigned int getColor() override; // Retorna el color del restaurant
    string getName() override { return PuntDeInteresBase::getName(); }

    // Setters
    void setTipus(const string& tipus) { m_tipusCuisine = tipus; }
    void setWheel(const string& rodes) { m_wheelchair = rodes; }
    void setGeneral(const string& amenity, const string& wheel, const string& tipus)
    {
        m_tipusCuisine = tipus;
        m_wheelchair = wheel;
    }

private:
    // Atributs privats
    string m_tipusCuisine;  // Tipus de cuina
    string m_wheelchair;    // Accessibilitat
};

/* --------------------------------------------------
 *  lp project - mapa / camins / pdis
 *  arnau baeza muñoz        niu: 1708086
 *  felipe tenorio da silva  niu: 1708283
 * --------------------------------------------------
 */