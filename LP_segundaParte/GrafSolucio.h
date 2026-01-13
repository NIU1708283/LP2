#pragma once

#include "Util.h"
#include "pch.h" // Caronte
#include "MapaBase.h"
#include <vector>
#include <stack>
#include <algorithm>
#include <cfloat>

using namespace std;


// grafo auxiliar para calcular caminos
class GrafSolucio
{
public:
	// constructor basico
	GrafSolucio() {
		m_nNodes = 0;
		m_nArestes = 0;
	}

	// construye el grafo a partir del mapa
	GrafSolucio(MapaBase* map);

	// destructor
	~GrafSolucio();
	
	 // Metodes

	// añade una arista entre dos nodos
	void setAresta(int posN1, int posN2, float pes);

	// añade un nodo si no existe
	void setNode(const Coordinate& n);

	// devuelve todas las coordenadas del grafo
	vector<Coordinate> getCoordenades() const { return m_nodes; }

	// calcula el camino mas corto entre dos puntos
	void camiMesCurt(const Coordinate& n1,
					 const Coordinate& n2,
					 stack<Coordinate>& cami);

// Privat
private:
	vector<Coordinate> m_nodes;          // nodos del grafo
	size_t m_nNodes;                     // numero de nodos
	size_t m_nArestes;                   // numero de aristas
	vector<vector<float>> m_matriuAdj;   // matriz de adyacencia

	// helpers internos
	bool estaEnVector(const vector<Coordinate>& nodes, const Coordinate& c);
	size_t minDist(vector<float>& dist, vector<bool>& visitat);
	void dijkstra(size_t n1, size_t n2, vector<float>& dist, vector<size_t>& ant);
};


/* --------------------------------------------------
 *  lp project - mapa / camins / pdis
 *  arnau baeza muñoz        niu: 1708086
 *  felipe tenorio da silva  niu: 1708283
 * --------------------------------------------------
 */
