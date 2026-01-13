#include "pch.h" // Caronte
#include "GrafSolucio.h"
#include <cfloat>

using namespace std;


// comprueba si una coordenada ya esta en el vector
bool GrafSolucio::estaEnVector(const vector<Coordinate>& nodes, const Coordinate& c)
{
	for (size_t i = 0; i < nodes.size(); i++) {
		if (nodes[i].lat == c.lat && nodes[i].lon == c.lon)
			return true;
	}
	return false;
}


GrafSolucio::~GrafSolucio()
{
	for (auto& fila : m_matriuAdj)
		fila.clear();

	m_matriuAdj.clear();
	m_nodes.clear();
} // si O SI


// construye el grafo a partir de los caminos del mapa
GrafSolucio::GrafSolucio(MapaBase* map)
{
	m_nArestes = 0;
	m_nNodes = 0;

	vector<CamiBase*> camins;
	map->getCamins(camins);

	for (int i = 0; i < camins.size(); i++) {
		vector<Coordinate> coords = camins[i]->getCamiCoords();

		for (int j = 0; j < coords.size(); j++) {
			setNode(coords[j]);

			// conectamos con el nodo anterior
			if (j > 0) {
				size_t pos1 = 0, pos2 = 0;

				// buscamos indices reales
				while (pos1 < m_nodes.size()) {
					if (m_nodes[pos1].lat == coords[j].lat &&
						m_nodes[pos1].lon == coords[j].lon)
						break;
					pos1++;
				}

				while (pos2 < m_nodes.size()) {
					if (m_nodes[pos2].lat == coords[j - 1].lat &&
						m_nodes[pos2].lon == coords[j - 1].lon)
						break;
					pos2++;
				}

				setAresta(pos1, pos2,
					Util::DistanciaHaversine(coords[j], coords[j - 1]));
			}
		}
	}

}


// Bidireccional esto
void GrafSolucio::setAresta(int posN1, int posN2, float pes)
{
	if (posN1 < m_nNodes && posN2 < m_nNodes) {
		m_matriuAdj[posN1][posN2] = pes;
		m_matriuAdj[posN2][posN1] = pes;
	}
}


// Add Node
void GrafSolucio::setNode(const Coordinate& n)
{
	if (!estaEnVector(m_nodes, n)) {
		m_nodes.push_back(n);
		m_matriuAdj.push_back(vector<float>(m_nNodes));
		m_nNodes++;

		for (int i = 0; i < m_nNodes; i++)
			m_matriuAdj[i].push_back(0);
	}
}


// devuelve el nodo no visitado con menor distancia
size_t GrafSolucio::minDist(vector<float>& dist, vector<bool>& visitat)
{
	float min = DBL_MAX;
	size_t minIndex = -1;

	for (size_t v = 0; v < m_nNodes; v++) {
		if (!visitat[v] && dist[v] <= min) {
			min = dist[v];
			minIndex = v;
		}
	}
	return minIndex;
}


// calcula el camino mas corto entre dos coordenadas
void GrafSolucio::camiMesCurt(const Coordinate& n1,
							 const Coordinate& n2,
							 stack<Coordinate>& cami)
{
	size_t pos1 = -1, pos2 = -1;

	// buscamos coincidencia exacta
	for (size_t i = 0; i < m_nodes.size(); i++) {
		if (m_nodes[i].lat == n1.lat && m_nodes[i].lon == n1.lon)
			pos1 = i;
		if (m_nodes[i].lat == n2.lat && m_nodes[i].lon == n2.lon)
			pos2 = i;
	}

	if (pos1 != -1 && pos2 != -1) {
		vector<float> dist;
		vector<size_t> ant;

		dijkstra(pos1, pos2, dist, ant);

		size_t it = pos2;
		cami.push(m_nodes[pos2]);

		while (it != pos1 && it != -1) {
			if (ant[it] == -1)
				break;
			cami.push(m_nodes[ant[it]]);
			it = ant[it];
		}
	}
}


// algoritmo de dijkstra clasico
void GrafSolucio::dijkstra(size_t n1,
						   size_t n2,
						   vector<float>& dist,
						   vector<size_t>& ant)
{
	vector<bool> visitat(m_nNodes, false);
	dist.assign(m_nNodes, DBL_MAX);
	ant.assign(m_nNodes, -1);

	dist[n1] = 0;

	for (size_t count = 0; count < m_nNodes - 1; count++) {
		size_t u = minDist(dist, visitat);
		if (u == -1 || dist[u] == DBL_MAX)
			break;

		visitat[u] = true;
		if (u == n2)
			return;

		for (size_t v = 0; v < m_nNodes; v++) {
			if (m_matriuAdj[u][v] != 0 && !visitat[v]) {
				if (dist[u] + m_matriuAdj[u][v] < dist[v]) {
					dist[v] = dist[u] + m_matriuAdj[u][v];
					ant[v] = u;
				}
			}
		}
	}
}


/* --------------------------------------------------
 *  lp project - mapa / camins / pdis
 *  arnau baeza muñoz        niu: 1708086
 *  felipe tenorio da silva  niu: 1708283
 * --------------------------------------------------
 */
