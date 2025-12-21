#include "pch.h"
#include "MapaSolucio.h"
#include "PuntDeInteresBotigaSolucio.h"
#include "PuntDeInteresRestaurantSolucio.h"

using namespace std;

void MapaSolucio::getPdis(std::vector<PuntDeInteresBase *> & pdis) {
    pdis = m_pdis;
}

void MapaSolucio::getCamins(std::vector<CamiBase *> & camins) {
    camins = m_camins;
}

void MapaSolucio::checkBuit()
{
    if (!m_camins.empty()) {
        m_camins.clear();
    }
    if (!m_pdis.empty()) {
        m_pdis.clear();
    }
}

void MapaSolucio::parsejaXmlElements(std::vector<XmlElement> &xmlElements) {
    // ---------------------------------------------------------
    // Implementació Part 1 (Parsing)
    // ---------------------------------------------------------
    vector<pair<string, Coordinate>> llista_referencies;
    pair<string, Coordinate> referencia;
    double latitud, longitud;
    int i, val_switch;
    bool triggerHighway, triggerTrobat;
    Coordinate c;
    string loc;

    checkBuit();

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
            case 1: { // Restaurant
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
            case 2: { // Botiga
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
            default: // Node genèric
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

    // ---------------------------------------------------------
    // INTEGRACIÓ PART 2: Construcció de Graf i BallTree
    // ---------------------------------------------------------
    if (m_graf) { delete m_graf; m_graf = new GrafSolucio(); }
    if (m_ballTree) { delete m_ballTree; m_ballTree = nullptr; }

    std::vector<Coordinate> puntsBallTree;

    for (CamiBase* cami : m_camins) {
        std::vector<Coordinate> coords = cami->getCamiCoords();
        if (coords.empty()) continue;

        for (size_t i = 0; i < coords.size(); i++) {
            puntsBallTree.push_back(coords[i]);
            if (i < coords.size() - 1) {
                m_graf->afegirAresta(coords[i], coords[i + 1]);
            }
        }
    }

    m_ballTree = new BallTree();
    m_ballTree->construirArbre(puntsBallTree);
}

CamiBase * MapaSolucio::buscaCamiMesCurt(PuntDeInteresBase *desde, PuntDeInteresBase *a) {
    if (!desde || !a || !m_ballTree || !m_graf) return nullptr;

    Coordinate Q_inici = { 0.0, 0.0 };
    // Passem l'arrel explícitament
    Coordinate iniciNode = m_ballTree->nodeMesProper(desde->getCoord(), Q_inici, m_ballTree->getArrel());

    Coordinate Q_final = { 0.0, 0.0 };
    Coordinate finalNode = m_ballTree->nodeMesProper(a->getCoord(), Q_final, m_ballTree->getArrel());

    // Executem Dijkstra amb els nodes trobats
    std::vector<Coordinate> camiCoords = m_graf->dijkstra(iniciNode, finalNode);

    if (camiCoords.empty()) return nullptr;

    return new CamiSolucio(camiCoords);
}