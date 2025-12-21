#include "pch.h" // Caronte
#include "MapaSolucio.h"
#include "PuntDeInteresBotigaSolucio.h"
#include "PuntDeInteresRestaurantSolucio.h"
#include "CamiSolucio.h"
#include "GrafSolucio.h"
#include "BallTree.h"
#include <stack>

using namespace std;


MapaSolucio::~MapaSolucio() {
	checkBuit();
}


// devuelve los puntos de interes
void MapaSolucio::getPdis(vector<PuntDeInteresBase*>& pdis) {
	pdis = m_pdis;
}


// devuelve los caminos
void MapaSolucio::getCamins(vector<CamiBase*>& camins) {
	camins = m_camins;
}


// limpia estructuras internas
void MapaSolucio::checkBuit() {
	if (!m_camins.empty()) {
		for (auto c : m_camins)
			delete c;
		m_camins.clear();
	}

	if (!m_pdis.empty()) {
		for (auto p : m_pdis)
			delete p;
		m_pdis.clear();
	}
}


// parsea el xml y crea pdis y camins
void MapaSolucio::parsejaXmlElements(vector<XmlElement>& xmlElements) {
	checkBuit();

	vector<pair<string, Coordinate>> llista_referencies;
	pair<string, Coordinate> referencia;

	double latitud, longitud;
	int i, tipus_node;
	bool esHighway, trobat;
	Coordinate c;
	string id_ref;

	for (auto actual = xmlElements.begin(); actual != xmlElements.end(); actual++) {
		latitud = 0;
		longitud = 0;

		// tractem nodes
		if (actual->id_element == "node") {
			tipus_node = 0;

			// detectem tipus
			for (i = 0; i < actual->fills.size(); i++) {
				if (actual->fills[i].first == "tag") {
					auto tag = Util::kvDeTag(actual->fills[i].second);
					if (tag.second == "restaurant" || tag.second == "cafe")
						tipus_node = 1;
					else if (tag.first == "shop")
						tipus_node = 2;
				}
			}

			switch (tipus_node) {

			// restaurant
			case 1: {
				string nom, cuisine, rodes;

				for (i = 0; i < actual->fills.size(); i++) {
					if (actual->fills[i].first == "tag") {
						auto tag = Util::kvDeTag(actual->fills[i].second);
						if (tag.first == "name") nom = tag.second;
						else if (tag.first == "cuisine") cuisine = tag.second;
						else if (tag.first == "wheelchair") rodes = tag.second;
					}
				}

				for (i = 0; i < actual->atributs.size(); i++) {
					if (actual->atributs[i].first == "lat") c.lat = stod(actual->atributs[i].second);
					if (actual->atributs[i].first == "lon") c.lon = stod(actual->atributs[i].second);
				}

				if (!nom.empty())
					m_pdis.push_back(new PuntDeInteresRestaurantSolucio(c, nom, cuisine, rodes));

				break;
			}

			// botiga
			case 2: {
				string nom, rodes, hores, tag_shop;

				for (i = 0; i < actual->fills.size(); i++) {
					if (actual->fills[i].first == "tag") {
						auto tag = Util::kvDeTag(actual->fills[i].second);
						if (tag.first == "name") nom = tag.second;
						else if (tag.first == "wheelchair") rodes = tag.second;
						else if (tag.first == "opening_hours") hores = tag.second;
						else if (tag.first == "shop") tag_shop = tag.second;
					}
				}

				for (i = 0; i < actual->atributs.size(); i++) {
					if (actual->atributs[i].first == "lat") c.lat = stod(actual->atributs[i].second);
					if (actual->atributs[i].first == "lon") c.lon = stod(actual->atributs[i].second);
				}

				if (!nom.empty())
					m_pdis.push_back(new PuntDeInteresBotigaSolucio(c, nom, tag_shop, hores, rodes));

				break;
			}

			// node normal (referencia)
			default:
				for (i = 0; i < actual->atributs.size(); i++) {
					if (actual->atributs[i].first == "id") id_ref = actual->atributs[i].second;
					if (actual->atributs[i].first == "lat") c.lat = stod(actual->atributs[i].second);
					if (actual->atributs[i].first == "lon") c.lon = stod(actual->atributs[i].second);
				}

				trobat = false;
				for (auto& r : llista_referencies) {
					if (r.first == id_ref) {
						trobat = true;
						break;
					}
				}

				if (!trobat) {
					referencia.first = id_ref;
					referencia.second = c;
					llista_referencies.push_back(referencia);
				}
				break;
			}
		}

		// tractem ways
		if (actual->id_element == "way") {
			esHighway = false;
			vector<Coordinate> cami;

			for (i = 0; i < actual->fills.size(); i++) {
				if (actual->fills[i].first == "nd") {
					for (auto& ref : llista_referencies) {
						if (ref.first == actual->fills[i].second[0].second)
							cami.push_back(ref.second);
					}
				}

				if (actual->fills[i].first == "tag") {
					auto tag = Util::kvDeTag(actual->fills[i].second);
					if (tag.first == "highway")
						esHighway = true;
				}
			}

			if (esHighway)
				m_camins.push_back(new CamiSolucio(cami, true));
		}
	}

	// dev //
}


// busca el camino mas corto entre dos puntos
CamiBase* MapaSolucio::buscaCamiMesCurt(PuntDeInteresBase* desde, PuntDeInteresBase* a) {
	if (!desde || !a)
		return nullptr;

	// construimos estructuras auxiliares en local
	GrafSolucio graf(this);

	BallTree ball;
	ball.construirArbre(graf.getCoordenades());

	// buscamos nodos mas cercanos
	Coordinate inici{ 0.0, 0.0 };
	Coordinate final{ 0.0, 0.0 };

	ball.nodeMesProper(desde->getCoord(), inici, &ball);
	ball.nodeMesProper(a->getCoord(), final, &ball);

	// calculamos camino
	vector<Coordinate> coordsCami;
	stack<Coordinate> pila;

	graf.camiMesCurt(inici, final, pila);

	if (pila.empty())
		return nullptr;

	while (!pila.empty()) {
		coordsCami.push_back(pila.top());
		pila.pop();
	}

	return new CamiSolucio(coordsCami, false);
}


/* --------------------------------------------------
 *  lp project - mapa / camins / pdis
 *  arnau baeza muñoz        niu: 1708086
 *  felipe tenorio da silva  niu: 1708283
 * --------------------------------------------------
 */
