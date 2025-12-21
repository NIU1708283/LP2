#pragma once
#include "PuntDeInteresBase.h"
#include <string>

using namespace std;

// Classe que representa una botiga amb informació addicional
class PuntDeInteresBotigaSolucio : public PuntDeInteresBase
{
public:
    // Constructors
    PuntDeInteresBotigaSolucio()
        : m_tagBotiga(""), m_openingHours(""), m_wheelchair("") {}

    // Constructor còpia
    PuntDeInteresBotigaSolucio(const PuntDeInteresBotigaSolucio& p)
        : PuntDeInteresBase(static_cast<const PuntDeInteresBase&>(p)),
          m_tagBotiga(p.m_tagBotiga),
          m_openingHours(p.m_openingHours),
          m_wheelchair(p.m_wheelchair) {}

    // Constructor amb paràmetres
    PuntDeInteresBotigaSolucio(Coordinate c, const string& nom,
                               const string& tag,
                               const string& openingHours,
                               const string& wheelchair)
        : PuntDeInteresBase(c, nom),
          m_tagBotiga(tag),
          m_openingHours(openingHours),
          m_wheelchair(wheelchair) {}

    // Getters
    string getTag() const { return m_tagBotiga; }
    string getOpeningHours() const { return m_openingHours; }
    string getWheelchair() const { return m_wheelchair; }

    // Retorna el color segons el tipus de botiga
    unsigned int getColor() override;

    // Setters
    void setTag(const string& tag) { m_tagBotiga = tag; }
    void setHours(const string& hours) { m_openingHours = hours; }
    void setWheel(const string& wheels) { m_wheelchair = wheels; }
    void setGeneral(const string& tag, const string& hours, const string& wheels)
    {
        m_tagBotiga = tag;
        m_openingHours = hours;
        m_wheelchair = wheels;
    }

private:
    string m_tagBotiga;     // Tipus de botiga (supermarket, tobacco, bakery...)
    string m_openingHours;  // Horari d'obertura
    string m_wheelchair;    // Accessibilitat ("yes"/"no")
};

// ---------------------------------------------------------------------------
// Fitxer modificat per Arnau Baeza (NIU 1708086) i Felipe Tenorio da Silva
// ---------------------------------------------------------------------------