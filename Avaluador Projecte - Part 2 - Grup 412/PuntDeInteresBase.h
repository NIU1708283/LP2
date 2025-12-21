#pragma once

#include "pch.h" // Caronte
#include "Common.h"
#include <string>

using namespace std;


// clase base de cualquier punto de interes
class PuntDeInteresBase {

public:
	// Construct
	PuntDeInteresBase() {m_coord = Coordinate{ 0.0, 0.0 }, m_name = "undefinit";} // valor base por si no viene nombre;
	PuntDeInteresBase(Coordinate coord, string name) {	m_coord = coord, m_name = name;}

	// Getters
	virtual string getName() {return m_name;} // V itual
	Coordinate getCoord() {return m_coord;}
	virtual unsigned int getColor() {return 0xFFA500;} // color por defecto
	
private:
	Coordinate m_coord;   // posicion del pdi
	string m_name;        // nombre visible

};


/* --------------------------------------------------
 *  lp project - mapa / camins / pdis
 *  arnau baeza muñoz        niu: 1708086
 *  felipe tenorio da silva  niu: 1708283
 * --------------------------------------------------
 */
