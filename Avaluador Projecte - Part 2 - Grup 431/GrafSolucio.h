#pragma once
#include "Util.h"
#include "MapaSolucio.h"
#include <stack>
#include <algorithm>
using namespace std;

class GrafSolucio
{
public:
	////////Constructors
	GrafSolucio() { m_nNodes = 0; m_nArestes = 0; } //Constructor per defecte
	GrafSolucio(MapaBase* map); // Constructor amb un mapa
	GrafSolucio(const vector<Coordinate>& nodes, const vector<vector<float>>& matriuAdj); //Constructor amb nodes i matriu d'adjacencia
	GrafSolucio(const vector<Coordinate>& nodes, const vector<vector<float>>& parelles_nodes, const vector<float>& pesos); //Constructor amb nodes i parelles de nodes amb pesos
	
	~GrafSolucio(); //Destructor

	int getNumNodes() { return m_nNodes; } 
	void setAresta(int posN1, int posN2, float pes);
	void setNode(const Coordinate& n);

	void crearMatriu(const vector<vector<float>>& parelles, const vector<float>& pesos);

	vector<Coordinate> getCoordenades() const { return m_nodes; }

	size_t minDist(vector<float>& dist, vector<bool>& visitat); //Retorna la distancia minima
	void camiMesCurt(const Coordinate& n1, const Coordinate& n2, stack<Coordinate>& cami); //Cami mes curt entre dos nodes
	void dijkstra(size_t n1, size_t n2, vector<float>& dist, vector<size_t>& ant); //Algorisme de Dijkstra "personalizado"

private:
	vector<Coordinate> m_nodes;
	size_t m_nNodes;
	size_t m_nArestes;
	vector<vector<float>> m_matriuAdj;
	bool estaEnVector(const vector<Coordinate>& nodes, const Coordinate& c);
};
