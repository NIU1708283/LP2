#include "pch.h"
#include "GrafSolucio.h"
#include <algorithm>

// Comparador personalitzat per a la cua de prioritat (Min-Heap)
// Només ens interessa la distància (el double), no la coordenada.
struct PQElementCompare {
    bool operator()(const std::pair<double, Coordinate>& a, const std::pair<double, Coordinate>& b) const {
        return a.first > b.first; // Més petit a dalt
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
    // Utilitzem el comparador personalitzat per evitar l'error de operator< a Coordinate
    typedef std::pair<double, Coordinate> ElementCua;
    std::priority_queue<ElementCua, std::vector<ElementCua>, PQElementCompare> pq;

    std::map<Coordinate, double, CoordinateCompare> distancies;
    std::map<Coordinate, Coordinate, CoordinateCompare> predecessors;

    // Inicialització
    // CORRECCIÓ: Bucle compatible amb C++ antic (sense structured bindings)
    for (auto const& pair : m_adjList) {
        Coordinate node = pair.first;
        distancies[node] = std::numeric_limits<double>::infinity();
    }
    
    if (m_adjList.find(origen) == m_adjList.end() || m_adjList.find(desti) == m_adjList.end()) {
        return {};
    }

    distancies[origen] = 0.0;
    pq.push({ 0.0, origen });

    while (!pq.empty()) {
        double distActual = pq.top().first;
        Coordinate u = pq.top().second;
        pq.pop();

        // Comparació de coordenades manual per evitar errors de precisió o operadors
        if (std::abs(u.lat - desti.lat) < 1e-9 && std::abs(u.lon - desti.lon) < 1e-9) {
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
    if (distancies[desti] == std::numeric_limits<double>::infinity()) {
        return cami;
    }

    Coordinate curr = desti;
    while (predecessors.find(curr) != predecessors.end()) {
        cami.push_back(curr);
        curr = predecessors[curr];
    }
    cami.push_back(origen);
    std::reverse(cami.begin(), cami.end());

    return cami;
}