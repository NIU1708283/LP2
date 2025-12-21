#pragma once
#include "Util.h"
#include "MapaBase.h" // Canviat de MapaSolucio.h a MapaBase.h per evitar dependencia circular
#include <stack>
#include <vector>
#include <algorithm>
#include <cfloat>

using namespace std;

class GrafSolucio
{
public:
    GrafSolucio() { m_nNodes = 0; m_nArestes = 0; }
    GrafSolucio(MapaBase* map); // Constructor clau que construeix el graf des del mapa
    
    ~GrafSolucio(); 

    void setAresta(int posN1, int posN2, float pes);
    void setNode(const Coordinate& n);

    vector<Coordinate> getCoordenades() const { return m_nodes; }

    void camiMesCurt(const Coordinate& n1, const Coordinate& n2, stack<Coordinate>& cami);

private:
    vector<Coordinate> m_nodes;
    size_t m_nNodes;
    size_t m_nArestes;
    vector<vector<float>> m_matriuAdj;
    
    bool estaEnVector(const vector<Coordinate>& nodes, const Coordinate& c);
    void crearMatriu(const vector<vector<float>>& parelles, const vector<float>& pesos);
    size_t minDist(vector<float>& dist, vector<bool>& visitat);
    void dijkstra(size_t n1, size_t n2, vector<float>& dist, vector<size_t>& ant);
};