// NEW PART 2
#include "pch.h"
#include "MapaSolucio.h"
using namespace std;

#include "PuntDeInteresBotigaSolucio.h" 
#include "PuntDeInteresRestaurantSolucio.h"

void MapaSolucio::getPdis(std::vector<PuntDeInteresBase *> & pdis) {
    pdis.clear();
	for (auto it = m_pdis.begin(); it != m_pdis.end(); it++)
		pdis.push_back(*it);
}

void MapaSolucio::getCamins(std::vector<CamiBase *> & camins) {
    camins.clear();
	for (auto it = m_camins.begin(); it != m_camins.end(); it++) {
		camins.push_back(*it);
	}
}

// Neteja contenidors si ja tenen dades
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
    // AQUI HAURIA D'ANAR LA TEVA IMPLEMENTACIÓ DE LA PART 1.
    // SI NO TENS ELS FITXERS DE BOTIGA/RESTAURANT, ASSEGURA'T QUE AQUESTA FUNCIÓ
    // ALMENYS OMPLE m_camins PERQUÈ LA PART 2 FUNCIONI.
// Referències de nodes: id -> coordenada
	vector<pair<string, Coordinate>> llista_referencies;
	pair<string, Coordinate> referencia;

	// Variables temporals
	double latitud, longitud;
	int i, val_switch;
	bool triggerHighway, triggerTrobat;
	Coordinate c;
	string loc;

	checkBuit(); // assegurem vectors buits

	for (auto actual = xmlElements.begin(); actual != xmlElements.end(); actual++) {
		latitud = 0; longitud = 0;

		// Tractament de nodes individuals (node)
		if ((*actual).id_element == "node") {
			val_switch = 0;

			// Mirem si és restaurant/cafè o botiga
			for (i = 0; i < (*actual).fills.size(); i++) {
				if ((*actual).fills[i].first == "tag") {
					pair<string, string> tagextret = Util::kvDeTag((*actual).fills[i].second);
					if ((tagextret.second == "restaurant") || (tagextret.second == "cafe")) {
						val_switch = 1;
					}
					else if (tagextret.first == "shop") {
						val_switch = 2;
					}
				}
			}

			// Segons tipus, extraiem dades i creem objectes
			switch (val_switch) {
			case 1: { // RESTAURANT
				string nom, cuisine, rodes;

				// Dades del restaurant
				for (i = 0; i < (*actual).fills.size(); i++) {
					if ((*actual).fills[i].first == "tag") {
						pair<string, string> tagextret = Util::kvDeTag((*actual).fills[i].second);
						if (tagextret.first == "name")       nom = tagextret.second;
						else if (tagextret.first == "cuisine")    cuisine = tagextret.second;
						else if (tagextret.first == "wheelchair") rodes = tagextret.second;
					}
				}
				// Coordenades
				for (i = 0; i < (*actual).atributs.size(); i++) {
					if ((*actual).atributs[i].first == "lat") { latitud = stod((*actual).atributs[i].second); c.lat = latitud; }
					if ((*actual).atributs[i].first == "lon") { longitud = stod((*actual).atributs[i].second); c.lon = longitud; }
				}
				// Afegim si té nom
				if (nom != "")
					m_pdis.push_back(new PuntDeInteresRestaurantSolucio(c, nom, cuisine, rodes));
				break;
			}

			case 2: { // BOTIGA
				string nom, rodes, hores, tag;

				// Dades de la botiga
				for (i = 0; i < (*actual).fills.size(); i++) {
					if ((*actual).fills[i].first == "tag") {
						pair<string, string> tagextret = Util::kvDeTag((*actual).fills[i].second);
						if (tagextret.first == "name")             nom = tagextret.second;
						else if (tagextret.first == "wheelchair")  rodes = tagextret.second;
						else if (tagextret.first == "opening_hours") hores = tagextret.second;
						else if (tagextret.first == "shop")        tag = tagextret.second;
					}
				}
				// Coordenades
				for (i = 0; i < (*actual).atributs.size(); i++) {
					if ((*actual).atributs[i].first == "lat") { latitud = stod((*actual).atributs[i].second); c.lat = latitud; }
					if ((*actual).atributs[i].first == "lon") { longitud = stod((*actual).atributs[i].second); c.lon = longitud; }
				}
				// Afegim si té nom
				if (nom != "")
					m_pdis.push_back(new PuntDeInteresBotigaSolucio(c, nom, tag, hores, rodes));
				break;
			}

			default: // Node genèric: el guardem a la llista de referències
				for (i = 0; i < (*actual).atributs.size(); i++) {
					if ((*actual).atributs[i].first == "id")  loc = (*actual).atributs[i].second;
					if ((*actual).atributs[i].first == "lat") { latitud = stod((*actual).atributs[i].second); c.lat = latitud; }
					if ((*actual).atributs[i].first == "lon") { longitud = stod((*actual).atributs[i].second); c.lon = longitud; }
				}

				triggerTrobat = false;
				for (auto recorregut = llista_referencies.begin(); recorregut != llista_referencies.end(); recorregut++) {
					if ((*recorregut).first == loc) {
						triggerTrobat = true; break;
					}
				}
				if (!triggerTrobat) {
					referencia.first = loc;
					referencia.second = c;
					llista_referencies.push_back(referencia);
				}
				break;
			}
		}

		// Tractament de camins (way)
		triggerHighway = false;

		if ((*actual).id_element == "way") {
			vector<Coordinate> retornat; // coordenades del camí

			for (i = 0; i < (*actual).fills.size(); i++) {
				if ((*actual).fills[i].first == "nd") { // referència a node
					for (auto recorregut = llista_referencies.begin(); recorregut != llista_referencies.end(); recorregut++) {
						if (((*recorregut).first) == (*actual).fills[i].second[0].second) {
							retornat.push_back((*recorregut).second);
						}
					}
				}
				if ((*actual).fills[i].first == "tag") { // etiqueta del way
					pair<string, string> tagextret = Util::kvDeTag((*actual).fills[i].second);
					if (tagextret.first == "highway") {
						triggerHighway = true;
					}
				}
			}

			// Si és highway, l’afegim a la llista de camins
			if (triggerHighway) {
				m_camins.push_back(new CamiSolucio(retornat, triggerHighway));
			}
		}
	}
    // ---------------------------------------------------------
    // INTEGRACIÓ PART 2: Construcció de Graf i BallTree
    // ---------------------------------------------------------
    // Esborrem estructures anteriors si n'hi ha
    if (m_graf) { delete m_graf; m_graf = new GrafSolucio(); }
    if (m_ballTree) { delete m_ballTree; m_ballTree = nullptr; }

    std::vector<Coordinate> puntsBallTree;

    for (CamiBase* cami : m_camins) {
        std::vector<Coordinate> coords = cami->getCamiCoords();
        if (coords.empty()) continue;

        for (size_t i = 0; i < coords.size(); i++) {
            // Recollim punts per al BallTree
            puntsBallTree.push_back(coords[i]);

            // Creem arestes al Graf
            if (i < coords.size() - 1) {
                m_graf->afegirAresta(coords[i], coords[i + 1]);
            }
        }
    }

    // Construïm el BallTree amb tots els punts recollits
    m_ballTree = new BallTree();
    m_ballTree->construirArbre(puntsBallTree);
}

CamiBase * MapaSolucio::buscaCamiMesCurt(PuntDeInteresBase *desde, PuntDeInteresBase *a) {
    if (!desde || !a || !m_ballTree || !m_graf) return nullptr;

    // 1. Trobar node del camí més proper al PDI origen
    Coordinate Q_inici = m_ballTree->getPivot(); 
    Coordinate iniciNode = m_ballTree->nodeMesProper(desde->getCoord(), Q_inici, m_ballTree);

    // 2. Trobar node del camí més proper al PDI destí
    Coordinate Q_final = m_ballTree->getPivot();
    Coordinate finalNode = m_ballTree->nodeMesProper(a->getCoord(), Q_final, m_ballTree);

    // 3. Executar Dijkstra sobre el graf
    std::vector<Coordinate> camiCoords = m_graf->dijkstra(iniciNode, finalNode);

    if (camiCoords.empty()) return nullptr;

    // 4. Retornar el resultat
    return new CamiSolucio(camiCoords); 
}