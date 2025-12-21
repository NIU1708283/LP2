#pragma once
#include <vector>
#include <iostream>
#include "Util.h"
#include "CamiBase.h"
#include <unordered_map>

using namespace std;

class GrafSolucio {
public:
    GrafSolucio() {
        m_numNodes = 0;
        m_numArestes = 0;
    }
    GrafSolucio(const std::vector<CamiBase*>& camins) { 
        construirGraf(camins);
    }
    ~GrafSolucio() {}
    void afegirNode(const Coordinate& coord) {
        // if (m_nodeMap.find(coord) == m_nodeMap.end()) {
        //     m_nodes.push_back(coord);
        //     m_nodeMap[coord] = m_nodes.size() - 1;
        //     m_numNodes++;
        // }
    }

    void afegirAresta(const Coordinate& coord1, const Coordinate& coord2) {
        // if (m_nodeMap.find(coord1) != m_nodeMap.end() && m_nodeMap.find(coord2) != m_nodeMap.end()) {
        //     m_matriuAdj[m_nodeMap[coord1]][m_nodeMap[coord2]] = Util::DistanciaHaversine(coord1, coord2);
        //     m_matriuAdj[m_nodeMap[coord2]][m_nodeMap[coord1]] = Util::DistanciaHaversine(coord1, coord2);
        //     m_numArestes++;
        // }
    }

private:
    vector<Coordinate> m_nodes;
    vector<vector<double>> m_matriuAdj;
    // std::unordered_map<Coordinate, int> m_nodeMap;
    int m_numNodes;
    int m_numArestes;
    void construirGraf(const std::vector<CamiBase*>& camins){
        for (int i = 0; i < camins.size(); i++) {
            vector<Coordinate> coords = camins[i]->getCamiCoords();
            for (int j = 0; j < coords.size(); j++) {
                afegirNode(coords[j]);
            }
        }
        m_matriuAdj.resize(m_numNodes);
        for (int i = 0; i < m_numNodes; i++) {
            m_matriuAdj[i].resize(m_numNodes, 0);
        }
        for (int i = 0; i < camins.size(); i++) {
            vector<Coordinate> coords = camins[i]->getCamiCoords();
            for (int j = 0; j < coords.size() - 1; j++) {
                afegirAresta(coords[j], coords[j + 1]);
            }
        }
    }
};
