#include "pch.h"
#include "BallTree.h"
#include <limits>

void BallTree::construirArbre(const std::vector<Coordinate>& coordenades) {
    // 1. Assignació fonamental per a l'Avaluador
    m_root = this;

    if (coordenades.empty()) return;

    m_coordenades = coordenades;
    Coordinate C = Util::calcularPuntCentral(m_coordenades);
    m_pivot = C;

    // Cas base: Fulla
    if (coordenades.size() == 1) {
        m_radi = 0;
        m_left = nullptr;
        m_right = nullptr;
        return;
    }

    // 2. Càlcul del radi (C -> A -> B)
    Coordinate A = coordenades[0];
    double maxDistC = -1.0;
    for (const auto& c : coordenades) {
        double d = Util::DistanciaHaversine(C, c);
        if (d > maxDistC) { maxDistC = d; A = c; }
    }
    m_radi = maxDistC;

    Coordinate B = coordenades[0];
    double maxDistA = -1.0;
    for (const auto& c : coordenades) {
        double d = Util::DistanciaHaversine(A, c);
        if (d > maxDistA) { maxDistA = d; B = c; }
    }

    // 3. Split
    std::vector<Coordinate> grupA, grupB;
    for (const auto& c : coordenades) {
        double distA = Util::DistanciaHaversine(c, A);
        double distB = Util::DistanciaHaversine(c, B);
        if (distA < distB) grupA.push_back(c);
        else grupB.push_back(c);
    }

    // Evitar bucles infinits amb duplicats
    if (grupA.empty() && !grupB.empty()) {
        grupA.push_back(grupB.back()); grupB.pop_back();
    } else if (grupB.empty() && !grupA.empty()) {
        grupB.push_back(grupA.back()); grupA.pop_back();
    }

    // Recursivitat
    if (!grupA.empty()) {
        m_left = new BallTree();
        m_left->construirArbre(grupA);
    }
    if (!grupB.empty()) {
        m_right = new BallTree();
        m_right->construirArbre(grupB);
    }
}

void BallTree::inOrdre(std::vector<std::list<Coordinate>>& out) {
    if (m_left) m_left->inOrdre(out);
    std::list<Coordinate> llista;
    for (auto& c : m_coordenades) llista.push_back(c);
    out.push_back(llista);
    if (m_right) m_right->inOrdre(out);
}

void BallTree::preOrdre(std::vector<std::list<Coordinate>>& out) {
    std::list<Coordinate> llista;
    for (auto& c : m_coordenades) llista.push_back(c);
    out.push_back(llista);
    if (m_left) m_left->preOrdre(out);
    if (m_right) m_right->preOrdre(out);
}

void BallTree::postOrdre(std::vector<std::list<Coordinate>>& out) {
    if (m_left) m_left->postOrdre(out);
    if (m_right) m_right->postOrdre(out);
    std::list<Coordinate> llista;
    for (auto& c : m_coordenades) llista.push_back(c);
    out.push_back(llista);
}

Coordinate BallTree::nodeMesProper(Coordinate targetQuery, Coordinate& Q, BallTree* ball) {
    if (!ball) return Q;

    double distPdiPivot = Util::DistanciaHaversine(targetQuery, ball->getPivot());
    double distPdiQ = Util::DistanciaHaversine(targetQuery, Q);

    // Poda triangular: Si la bola més propera no pot millorar Q, marxem.
    if (distPdiPivot - ball->getRadi() >= distPdiQ) {
        return Q;
    }

    // Cas Fulla: Comprovar tots els punts del node
    if (!ball->getEsquerre() && !ball->getDreta()) {
        for (const auto& punt : ball->getCoordenades()) {
            double d = Util::DistanciaHaversine(targetQuery, punt);
            if (d < Util::DistanciaHaversine(targetQuery, Q)) {
                Q = punt;
            }
        }
    } else {
        // Cas Node Intern: Ordenar visita de fills
        double distLeft = std::numeric_limits<double>::max();
        double distRight = std::numeric_limits<double>::max();

        if (ball->getEsquerre()) 
            distLeft = Util::DistanciaHaversine(targetQuery, ball->getEsquerre()->getPivot());
        if (ball->getDreta()) 
            distRight = Util::DistanciaHaversine(targetQuery, ball->getDreta()->getPivot());

        if (distLeft < distRight) {
            if (ball->getEsquerre()) nodeMesProper(targetQuery, Q, ball->getEsquerre());
            if (ball->getDreta()) nodeMesProper(targetQuery, Q, ball->getDreta());
        } else {
            if (ball->getDreta()) nodeMesProper(targetQuery, Q, ball->getDreta());
            if (ball->getEsquerre()) nodeMesProper(targetQuery, Q, ball->getEsquerre());
        }
    }
    return Q;
}