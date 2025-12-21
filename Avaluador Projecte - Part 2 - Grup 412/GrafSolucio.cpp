#include "pch.h"
#include "GrafSolucio.h"
#include <algorithm>

// Comparador per la Priority Queue (Min-Heap)
struct PQElementCompare {
    bool operator()(const std::pair<double, Coordinate>& a, const std::pair<double, Coordinate>& b) const {
        return a.first > b.first;
    }
};

GrafSolucio::GrafSolucio() {}
GrafSolucio::~GrafSolucio() {}

void GrafSolucio::afegirAresta(const Coordinate& n1, const Coordinate& n2) {
    double distancia = Util::DistanciaHaversine(n1, n2);
    m_adjList[n1].push_back({ n2, distancia });
    m_adjList[n2].push_back({ n1, distancia });
}

std::vector<Coordinate> GrafSolucio::dijkstra(const Coordinate& origen, const Coordinate& desti) {
    typedef std::pair<double, Coordinate> ElementCua;
    std::priority_queue<ElementCua, std::vector<ElementCua>, PQElementCompare> pq;

    std::map<Coordinate, double, CoordinateCompare> distancies;
    std::map<Coordinate, Coordinate, CoordinateCompare> predecessors;

    // Inicialització
    for (auto const& pair : m_adjList) {
        distancies[pair.first] = std::numeric_limits<double>::infinity();
    }
    
    // Si nodes no existeixen (per seguretat)
    if (m_adjList.find(origen) == m_adjList.end() || m_adjList.find(desti) == m_adjList.end()) {
        return {};
    }

    distancies[origen] = 0.0;
    pq.push({ 0.0, origen });

    while (!pq.empty()) {
        double distActual = pq.top().first;
        Coordinate u = pq.top().second;
        pq.pop();

        // Comparació estricta (els doubles han de ser idèntics)
        if (u.lat == desti.lat && u.lon == desti.lon) {
            break;
        }

        if (distActual > distancies[u]) continue;

        for (auto& edge : m_adjList[u]) {
            Coordinate v = edge.first;
            double pes = edge.second;

            if (distancies[u] + pes < distancies[v]) {
                distancies[v] = distancies[u] + pes;
                predecessors[v] = u;
                pq.push({ distancies[v], v });
            }
        }
    }

    std::vector<Coordinate> cami;
    // Si no hem arribat (distància infinita)
    if (distancies[desti] == std::numeric_limits<double>::infinity()) {
        return cami;
    }

    // Reconstrucció
    Coordinate curr = desti;
    while (predecessors.find(curr) != predecessors.end()) {
        cami.push_back(curr);
        curr = predecessors[curr];
    }
    cami.push_back(origen);
    std::reverse(cami.begin(), cami.end());

    return cami;
}