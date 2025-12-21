#pragma once

#include "pch.h" // Caronte
#include "Common.h"
#include <cmath>
#include <vector>
#include <string>
#include <utility>
#include <fstream>

using namespace std;

// Aqui tocar poco

// utilidades matematicas y helpers del proyecto
class Util {
private:
#if defined(_MSC_VER)
	// solo para version grafica (fuera caronte)
	static const string m_logFileName;
#endif
	static double m_PI;
	static double m_RadiTerraX2;

public:
	Util();

	// conversion grados <-> radianes
	static double deg2Rad(double deg);
	static double rad2Deg(double rad);

	// extrae par k=v de un tag xml
	static pair<string, string> kvDeTag(vector<PAIR_ATTR_VALUE>& atributsTag);

#if defined(_MSC_VER)
	// debug visual (solo wpf)
	static void escriuEnMonitor(string text);
#endif

	// distancia entre dos puntos (haversine)
	static double DistanciaHaversine(double lat1, double lon1, double lat2, double lon2);
	static double DistanciaHaversine(Coordinate px1, Coordinate px2);

	// new part 2: calcula punto central de varios puntos
	static Coordinate calcularPuntCentral(vector<Coordinate>& punts);

};
 

/* --------------------------------------------------
 *  lp project - mapa / camins / pdis
 *  arnau baeza muñoz        niu: 1708086
 *  felipe tenorio da silva  niu: 1708283
 * --------------------------------------------------
 */
