#ifndef _UTIL_H
#define _UTIL_H

#include "pch.h"
#include "Common.h"
#include <math.h>
#include <cmath>
#include <fstream>

class Util {
private:
	static const std::string m_logFileName;
	static float m_PI;
	static float m_RadiTerraX2;

public:
	Util();

	static float deg2Rad(float deg);
	static float rad2Deg(float rad);

	static std::pair<std::string, std::string> kvDeTag(std::vector<PAIR_ATTR_VALUE>& atributsTag);
	static void escriuEnMonitor(std::string text);

	static float DistanciaHaversine(float lat1, float lon1, float lat2, float lon2);
	static float DistanciaHaversine(Coordinate px1, Coordinate px2);

	static Coordinate calcularPuntCentral(std::vector<Coordinate>& punts);

};

#endif



