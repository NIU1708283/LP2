#include "BallTree.h"
#include <limits>
#include <cfloat>
#include <stack>
#include "pch.h" // Caronte

using namespace std;


// busca el nodo mas cercano a pdi usando el ball tree
Coordinate BallTree::nodeMesProper(Coordinate pdi, Coordinate& Q, BallTree* ball)
{
	// si ya coincide, no hay nada que buscar
	if (pdi.lat == Q.lat && pdi.lon == Q.lon)
		return Q;

	// si no hay raiz real, forzamos actualizacion
	if (ball->getArrel() == nullptr) {
		Q = Coordinate{ 0.0, 0.0 };
	}

	// distancia del punto al pivot actual
	float d_pivot = Util::DistanciaHaversine(pdi, ball->m_pivot);

	// distancia del mejor candidato actual
	float d_q = Util::DistanciaHaversine(Q, ball->m_pivot);

	// poda: no puede haber mejor solucion dentro de esta bola
	if ((d_pivot - ball->m_radi) >= d_q)
		return Q;

	// si es hoja, comprobamos todas las coordenadas
	if (ball->m_left == nullptr && ball->m_right == nullptr) {
		for (auto& c : m_coordenades) {
			if (Util::DistanciaHaversine(pdi, c) < Util::DistanciaHaversine(pdi, Q)) {
				Q = c;
			}
		}
	}
	else {
		// decidimos por donde bajar primero
		float da = DBL_MAX;
		float db = DBL_MAX;

		if (ball->m_left != nullptr)
			da = Util::DistanciaHaversine(pdi, ball->m_left->m_pivot);
		if (ball->m_right != nullptr)
			db = Util::DistanciaHaversine(pdi, ball->m_right->m_pivot);

		// exploramos primero el lado mas prometedor
		if (da < db) {
			if (ball->m_left != nullptr)
				nodeMesProper(pdi, Q, ball->m_left);
			if (ball->m_right != nullptr)
				nodeMesProper(pdi, Q, ball->m_right);
		}
		else {
			if (ball->m_right != nullptr)
				nodeMesProper(pdi, Q, ball->m_right);
			if (ball->m_left != nullptr)
				nodeMesProper(pdi, Q, ball->m_left);
		}
	}

	// dev //
	return Q;
}

// Somos jardineros con tanta poda


// construye el arbol de forma recursiva
void BallTree::construirArbre(const vector<Coordinate>& coordenades)
{
	m_coordenades = coordenades;
	m_pivot = Util::calcularPuntCentral(m_coordenades);

	// caso base: una sola coordenada
	if (m_coordenades.size() <= 1) {
		m_left = nullptr;
		m_right = nullptr;
		m_radi = 0;
		return;
	}
	
	//if (m_coordenades.size() <= 1) {
	//	m_left = NULL;
	//	m_right = NULL;
	//	m_radi = 0.1;
	//	return;
	//} / Esto no va felipe :(

	// calculamos el radio (punto mas lejano al pivot)
	vector<float> distancies;
	size_t idx_llunya = 0;

	for (size_t i = 0; i < m_coordenades.size(); i++) {
		distancies.push_back(Util::DistanciaHaversine(m_pivot, m_coordenades[i]));
		if (distancies[i] > distancies[idx_llunya])
			idx_llunya = i;
	}

	m_radi = distancies[idx_llunya];

	// punto mas alejado del mas lejano
	Coordinate puntA = m_coordenades[idx_llunya];
	Coordinate puntB = m_coordenades[0];

	for (size_t i = 1; i < m_coordenades.size(); i++) {
		if (Util::DistanciaHaversine(puntB, puntA) <
			Util::DistanciaHaversine(m_coordenades[i], puntA)) {
			puntB = m_coordenades[i];
		}
	}

	// separamos las coordenadas en dos grupos
	vector<Coordinate> coordsA, coordsB;
	for (auto& c : m_coordenades) {
		if (Util::DistanciaHaversine(c, puntA) <
			Util::DistanciaHaversine(c, puntB))
			coordsA.push_back(c);
		else
			coordsB.push_back(c);
	}

	// creamos hijos
	m_left = new BallTree();
	m_right = new BallTree();

	m_left->m_root = this;
	m_right->m_root = this;

	// recursion
	m_left->construirArbre(coordsA);
	m_right->construirArbre(coordsB);
}


// recorrido inorden
void BallTree::inOrdre(vector<list<Coordinate>>& out)
{
	if (m_coordenades.empty())
		return;

	if (m_left != nullptr)
		m_left->inOrdre(out);

	out.emplace_back(m_coordenades.begin(), m_coordenades.end());

	if (m_right != nullptr)
		m_right->inOrdre(out);
}

// RECORRIDOS

// recorrido preorden
void BallTree::preOrdre(vector<list<Coordinate>>& out)
{
	if (m_coordenades.empty())
		return;

	out.emplace_back(m_coordenades.begin(), m_coordenades.end());

	if (m_left != nullptr)
		m_left->preOrdre(out);
	if (m_right != nullptr)
		m_right->preOrdre(out);
}


// recorrido postorden
void BallTree::postOrdre(vector<list<Coordinate>>& out)
{
	if (m_coordenades.empty())
		return;

	if (m_left != nullptr)
		m_left->postOrdre(out);
	if (m_right != nullptr)
		m_right->postOrdre(out);

	out.emplace_back(m_coordenades.begin(), m_coordenades.end());
}


// destructor: liberamos memoria recursivamente
BallTree::~BallTree()
{
	if (m_left != nullptr) {
		delete m_left;
		m_left = nullptr;
	}

	if (m_right != nullptr) {
		delete m_right;
		m_right = nullptr;
	}
	
	//Hay que vaciar que lo dijeron en clase
}


/* --------------------------------------------------
 *  lp project - mapa / camins / pdis
 *  arnau baeza muñoz        niu: 1708086
 *  felipe tenorio da silva  niu: 1708283
 * --------------------------------------------------
 */
