#pragma once
#include "pch.h"
#include "Common.h"
#include "Util.h"
#include <map>
#include <vector>
#include <queue>
#include <limits>

// Comparador per utilitzar Coordinate com a clau en mapes
struct CoordinateCompare {
    bool operator() (const Coordinate& lhs, const Coordinate& rhs) const {
        if (std::abs(lhs.lat - rhs.lat) > 1e-9) return lhs.lat < rhs.lat;
        return lhs.lon < rhs.lon - 1e-9;
    }
};

class GrafSolucio {
public:
    GrafSolucio();
    ~GrafSolucio();

    // Afegeix un node i una aresta entre dos punts amb el pes calculat
    void afegirAresta(const Coordinate& n1, const Coordinate& n2);

    // Algorisme de Dijkstra per trobar el camí més curt
    std::vector<Coordinate> dijkstra(const Coordinate& origen, const Coordinate& desti);

private:
    // Llista d'adjacència: Node -> Llista de parelles <Vei, Pes>
    std::map<Coordinate, std::vector<std::pair<Coordinate, double>>, CoordinateCompare> m_adjList;
};