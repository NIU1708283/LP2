#pragma once

#include "PuntDeInteresBase.h"
#include <string>
#include "pch.h" // Caronte

using namespace std;


// pdi de tipo botiga
class PuntDeInteresBotigaSolucio : public PuntDeInteresBase
{
public:
	// constructor base
	PuntDeInteresBotigaSolucio() {
		m_tagBotiga = "";
		m_openingHours = "";
		m_wheelchair = "";
	}

	// constructor copia
	PuntDeInteresBotigaSolucio(const PuntDeInteresBotigaSolucio& p)
		: PuntDeInteresBase(p) {
		m_tagBotiga = p.m_tagBotiga;
		m_openingHours = p.m_openingHours;
		m_wheelchair = p.m_wheelchair;
	}

	// constructor con datos
	PuntDeInteresBotigaSolucio(Coordinate c,
							   const string& nom,
							   const string& tag,
							   const string& openingHours,
							   const string& wheelchair)
		: PuntDeInteresBase(c, nom) {
		m_tagBotiga = tag;
		m_openingHours = openingHours;
		m_wheelchair = wheelchair;
	}

	// getters rapidos
	string getTag() const { return m_tagBotiga; }
	string getOpeningHours() const { return m_openingHours; }
	string getWheelchair() const { return m_wheelchair; }

	// calcula el color segun la botiga
	unsigned int getColor();

	// setters
	void setTag(const string& tag) { m_tagBotiga = tag; }
	void setHours(const string& hours) { m_openingHours = hours; }
	void setWheel(const string& wheels) { m_wheelchair = wheels; }
	void setGeneral(const string& tag,
					const string& hours,
					const string& wheels) {
		m_tagBotiga = tag;
		m_openingHours = hours;
		m_wheelchair = wheels;
	}

private:
	string m_tagBotiga;      // tipo de botiga
	string m_openingHours;   // horario
	string m_wheelchair;     // accesibilidad
};


/* --------------------------------------------------
 *  lp project - mapa / camins / pdis
 *  arnau baeza muñoz        niu: 1708086
 *  felipe tenorio da silva  niu: 1708283
 * --------------------------------------------------
 */
