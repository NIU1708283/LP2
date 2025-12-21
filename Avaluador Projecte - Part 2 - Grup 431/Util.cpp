#include "pch.h"

#include "Util.h"

float Util::m_PI = 3.141592653589793238462643383279502884L;
float Util::m_RadiTerraX2 = static_cast<float>(2 * 6371);
const std::string Util::m_logFileName = "Log.txt";

Util::Util() {

}


void Util::escriuEnMonitor(std::string text) {
    std::ofstream of(m_logFileName);
    of << text << std::endl;
    of.flush();
    of.close();
}

std::pair<std::string, std::string> Util::kvDeTag(std::vector<PAIR_ATTR_VALUE>& atributsTag) {
    std::pair<std::string, std::string> resultat = std::make_pair("", "");

    std::string k = "";
    std::string v = "";

    for (const PAIR_ATTR_VALUE& atributTag : atributsTag) {
        if (atributTag.first == "k") {
            k = atributTag.second;
        }
        else if (atributTag.first == "v") {
            v = atributTag.second;
        }
    }

    resultat.first = k;
    resultat.second = v;

    return resultat;
}

float Util::deg2Rad(float deg) {
    return deg * m_PI / 180.0f;
}

float Util::rad2Deg(float rad) {
    return rad * 180.0f / m_PI;
}


float Util::DistanciaHaversine(float lat1, float lon1, float lat2, float lon2) {
    lat1 = deg2Rad(lat1);
    lon1 = deg2Rad(lon1);

    lat2 = deg2Rad(lat2);
    lon2 = deg2Rad(lon2);

    float diffLat = lat1 - lat2;
    float diffLon = lon1 - lon2;

    float resultatHaversine = m_RadiTerraX2 * asin(sqrt(sin(diffLat / 2) * sin(diffLat / 2) + cos(lat1) * cos(lat2) * sin(diffLon / 2) * sin(diffLon / 2)));

    return std::abs(resultatHaversine);
}

float Util::DistanciaHaversine(Coordinate px1, Coordinate px2) {
    return DistanciaHaversine(px1.lat, px1.lon, px2.lat, px2.lon);
}

Coordinate Util::calcularPuntCentral(std::vector<Coordinate>& punts) {
    float x = 0.0f, y = 0.0f, z = 0.0f;
    for (Coordinate point : punts) {
        x += cos(Util::deg2Rad(point.lat)) * cos(Util::deg2Rad(point.lon));
        y += cos(Util::deg2Rad(point.lat)) * sin(Util::deg2Rad(point.lon));
        z += sin(Util::deg2Rad(point.lon));
    }

    x = x / punts.size();
    y = y / punts.size();
    z = z / punts.size();

    float lat = rad2Deg(atan2(y, z));
    float lon = rad2Deg(atan2(z, sqrt(x * x + y * y)));

    return Coordinate{ lat, lon };
}


