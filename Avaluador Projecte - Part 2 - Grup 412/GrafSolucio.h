#pragma once
#include "pch.h"
#include "Common.h"
#include "Util.h"
#include <map>
#include <vector>
#include <queue>
#include <limits>

// Comparador estricte necessari per a que std::map funcioni bé amb Coordinate
struct CoordinateCompare {
    bool operator() (const Coordinate& lhs, const Coordinate& rhs) const {
        if (lhs.lat != rhs.lat) return lhs.lat < rhs.lat;
        return lhs.lon < rhs.lon;
    }
};

class GrafSolucio {
public:
    GrafSolucio();
    ~GrafSolucio();

    void afegirAresta(const Coordinate& n1, const Coordinate& n2);
    std::vector<Coordinate> dijkstra(const Coordinate& origen, const Coordinate& desti);

private:
    std::map<Coordinate, std::vector<std::pair<Coordinate, double>>, CoordinateCompare> m_adjList;
};