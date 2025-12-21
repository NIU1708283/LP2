#include "pch.h"
#include "MapaSolucio.h"
#include "PuntDeInteresBotigaSolucio.h"
#include "PuntDeInteresRestaurantSolucio.h"
#include "CamiSolucio.h"
#include "GrafSolucio.h" // Incloure aquí
#include "BallTree.h"    // Incloure aquí
#include <stack>

using namespace std;

MapaSolucio::~MapaSolucio() {
    checkBuit();
}

void MapaSolucio::getPdis(std::vector<PuntDeInteresBase *> & pdis) {
    pdis = m_pdis;
}

void MapaSolucio::getCamins(std::vector<CamiBase *> & camins) {
    camins = m_camins;
}

void MapaSolucio::checkBuit() {
    if (!m_camins.empty()) {
        for(auto c : m_camins) delete c;
        m_camins.clear();
    }
    if (!m_pdis.empty()) {
        for(auto p : m_pdis) delete p;
        m_pdis.clear();
    }
}

void MapaSolucio::parsejaXmlElements(std::vector<XmlElement> &xmlElements) {
    checkBuit();
    // Només parsejem PDI i Camins. NO construim graf ni arbre aquí.
    
    vector<pair<string, Coordinate>> llista_referencies;
    pair<string, Coordinate> referencia;
    double latitud, longitud;
    int i, val_switch;
    bool triggerHighway, triggerTrobat;
    Coordinate c;
    string loc;

    for (auto actual = xmlElements.begin(); actual != xmlElements.end(); actual++) {
        latitud = 0; longitud = 0;
        if ((*actual).id_element == "node") {
            val_switch = 0;
            for (i = 0; i < (*actual).fills.size(); i++) {
                if ((*actual).fills[i].first == "tag") {
                    pair<string, string> tagextret = Util::kvDeTag((*actual).fills[i].second);
                    if ((tagextret.second == "restaurant") || (tagextret.second == "cafe")) val_switch = 1;
                    else if (tagextret.first == "shop") val_switch = 2;
                }
            }
            switch (val_switch) {
            case 1: { 
                string nom, cuisine, rodes;
                for (i = 0; i < (*actual).fills.size(); i++) {
                    if ((*actual).fills[i].first == "tag") {
                        pair<string, string> tagextret = Util::kvDeTag((*actual).fills[i].second);
                        if (tagextret.first == "name") nom = tagextret.second;
                        else if (tagextret.first == "cuisine") cuisine = tagextret.second;
                        else if (tagextret.first == "wheelchair") rodes = tagextret.second;
                    }
                }
                for (i = 0; i < (*actual).atributs.size(); i++) {
                    if ((*actual).atributs[i].first == "lat") { latitud = stod((*actual).atributs[i].second); c.lat = latitud; }
                    if ((*actual).atributs[i].first == "lon") { longitud = stod((*actual).atributs[i].second); c.lon = longitud; }
                }
                if (nom != "") m_pdis.push_back(new PuntDeInteresRestaurantSolucio(c, nom, cuisine, rodes));
                break;
            }
            case 2: { 
                string nom, rodes, hores, tag;
                for (i = 0; i < (*actual).fills.size(); i++) {
                    if ((*actual).fills[i].first == "tag") {
                        pair<string, string> tagextret = Util::kvDeTag((*actual).fills[i].second);
                        if (tagextret.first == "name") nom = tagextret.second;
                        else if (tagextret.first == "wheelchair") rodes = tagextret.second;
                        else if (tagextret.first == "opening_hours") hores = tagextret.second;
                        else if (tagextret.first == "shop") tag = tagextret.second;
                    }
                }
                for (i = 0; i < (*actual).atributs.size(); i++) {
                    if ((*actual).atributs[i].first == "lat") { latitud = stod((*actual).atributs[i].second); c.lat = latitud; }
                    if ((*actual).atributs[i].first == "lon") { longitud = stod((*actual).atributs[i].second); c.lon = longitud; }
                }
                if (nom != "") m_pdis.push_back(new PuntDeInteresBotigaSolucio(c, nom, tag, hores, rodes));
                break;
            }
            default: 
                for (i = 0; i < (*actual).atributs.size(); i++) {
                    if ((*actual).atributs[i].first == "id") loc = (*actual).atributs[i].second;
                    if ((*actual).atributs[i].first == "lat") { latitud = stod((*actual).atributs[i].second); c.lat = latitud; }
                    if ((*actual).atributs[i].first == "lon") { longitud = stod((*actual).atributs[i].second); c.lon = longitud; }
                }
                triggerTrobat = false;
                for (auto recorregut = llista_referencies.begin(); recorregut != llista_referencies.end(); recorregut++) {
                    if ((*recorregut).first == loc) { triggerTrobat = true; break; }
                }
                if (!triggerTrobat) {
                    referencia.first = loc; referencia.second = c;
                    llista_referencies.push_back(referencia);
                }
                break;
            }
        }
        if ((*actual).id_element == "way") {
            triggerHighway = false;
            vector<Coordinate> retornat;
            for (i = 0; i < (*actual).fills.size(); i++) {
                if ((*actual).fills[i].first == "nd") {
                    for (auto recorregut = llista_referencies.begin(); recorregut != llista_referencies.end(); recorregut++) {
                        if (((*recorregut).first) == (*actual).fills[i].second[0].second) {
                            retornat.push_back((*recorregut).second);
                        }
                    }
                }
                if ((*actual).fills[i].first == "tag") {
                    pair<string, string> tagextret = Util::kvDeTag((*actual).fills[i].second);
                    if (tagextret.first == "highway") triggerHighway = true;
                }
            }
            if (triggerHighway) {
                m_camins.push_back(new CamiSolucio(retornat, triggerHighway));
            }
        }
    }
}

// Lógica del Grupo 431: Construir estructuras locales
CamiBase * MapaSolucio::buscaCamiMesCurt(PuntDeInteresBase *desde, PuntDeInteresBase *a) {
    if (!desde || !a) return nullptr;

    // 1. Construir Graf i BallTree localment (Stack allocated)
    GrafSolucio graf(this); 
    
    BallTree ball; 
    ball.construirArbre(graf.getCoordenades()); 

    // 2. Cercar nodes més propers
    Coordinate inici, final;
    Coordinate Q_dummy = {0.0, 0.0};
    
    // Usem la teva implementació de nodeMesProper, passant &ball com a root
    ball.nodeMesProper(desde->getCoord(), inici, &ball);
    
    // Reiniciem Q per la segona cerca (encara que sigui per valor, per claredat)
    Q_dummy = {0.0, 0.0};
    ball.nodeMesProper(a->getCoord(), final, &ball);

    // 3. Calcular camí Dijkstra
    vector<Coordinate> qCami;
    stack<Coordinate> pilaQCami;
    
    graf.camiMesCurt(inici, final, pilaQCami);

    if (pilaQCami.empty()) return nullptr;

    while (!pilaQCami.empty()) {
        qCami.push_back(pilaQCami.top());
        pilaQCami.pop();
    }

    return new CamiSolucio(qCami, false);
}