#include "pch.h"
#include "BallTree.h"
#include <limits>
#include <cfloat> 
#include <cmath>

BallTree::~BallTree() {
    if (m_left != nullptr) { delete m_left; m_left = nullptr; }
    if (m_right != nullptr) { delete m_right; m_right = nullptr; }
}

void BallTree::construirArbre(const std::vector<Coordinate>& coordenades) {
    // 1. Assignació fonamental perquè l'Avaluador trobi l'arbre
    m_root = this;

    if (coordenades.empty()) return;

    m_coordenades = coordenades;
    m_pivot = Util::calcularPuntCentral(m_coordenades);

    // Cas base: Fulla (si té 1 element)
    if (coordenades.size() == 1) {
        m_radi = 0;
        m_left = nullptr;
        m_right = nullptr;
        return;
    }

    // Calcular radi (distància al punt més llunyà del centre)
    Coordinate A = coordenades[0];
    double maxDistC = -1.0;
    for (const auto& c : coordenades) {
        double d = Util::DistanciaHaversine(m_pivot, c);
        if (d > maxDistC) { maxDistC = d; A = c; }
    }
    m_radi = maxDistC;

    // Calcular punt B per dividir (el més llunyà a A)
    Coordinate B = coordenades[0];
    double maxDistA = -1.0;
    for (const auto& c : coordenades) {
        double d = Util::DistanciaHaversine(A, c);
        if (d > maxDistA) { maxDistA = d; B = c; }
    }

    // Dividir els punts en dos grups
    std::vector<Coordinate> grupA, grupB;
    for (const auto& c : coordenades) {
        if (Util::DistanciaHaversine(c, A) < Util::DistanciaHaversine(c, B))
            grupA.push_back(c);
        else
            grupB.push_back(c);
    }

    // Gestionar duplicats o punts molt propers per evitar bucles infinits
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
    std::list<Coordinate> llista(m_coordenades.begin(), m_coordenades.end());
    out.push_back(llista);
    if (m_right) m_right->inOrdre(out);
}

void BallTree::preOrdre(std::vector<std::list<Coordinate>>& out) {
    std::list<Coordinate> llista(m_coordenades.begin(), m_coordenades.end());
    out.push_back(llista);
    if (m_left) m_left->preOrdre(out);
    if (m_right) m_right->preOrdre(out);
}

void BallTree::postOrdre(std::vector<std::list<Coordinate>>& out) {
    if (m_left) m_left->postOrdre(out);
    if (m_right) m_right->postOrdre(out);
    std::list<Coordinate> llista(m_coordenades.begin(), m_coordenades.end());
    out.push_back(llista);
}

// Implementació inspirada en el Grup 431
Coordinate BallTree::nodeMesProper(Coordinate targetQuery, Coordinate& Q, BallTree* ball) {
    if (!ball) return Q;

    // Inicialitzar Q si és la raíz (Q inicial és 0,0)
    if (ball->getArrel() == nullptr) {
        Q.lat = 0;
        Q.lon = 0;
    }

    // Si hem trobat exactament el punt, retornem
    if (targetQuery.lat == Q.lat && targetQuery.lon == Q.lon) return Q;

    // PAS 1: Distància del Target al Pivot de la bola actual
    double D1 = Util::DistanciaHaversine(targetQuery, ball->getPivot());

    // PAS 2: Distància de Q al Pivot (Lògica Grup 431)
    double D2 = Util::DistanciaHaversine(Q, ball->getPivot());

    // PAS 3: Condició de Poda del Grup 431
    // Es compara (D1 - Radi) >= D2.
    if ((D1 - ball->getRadi()) >= D2) {
        return Q;
    }

    // PAS 4: Cas FULLA
    if (!ball->getEsquerre() && !ball->getDreta()) {
        for (const auto& punt : ball->getCoordenades()) {
            double distTargetPunt = Util::DistanciaHaversine(targetQuery, punt);
            double distTargetQ = Util::DistanciaHaversine(targetQuery, Q);

            if (distTargetPunt < distTargetQ) {
                Q = punt;
            }
        }
    } else {
        // PAS 5: Cas NODE INTERN (Recursivitat ordenada)
        double distLeft = DBL_MAX;
        double distRight = DBL_MAX;

        if (ball->getEsquerre())
            distLeft = Util::DistanciaHaversine(targetQuery, ball->getEsquerre()->getPivot());
        if (ball->getDreta())
            distRight = Util::DistanciaHaversine(targetQuery, ball->getDreta()->getPivot());

        if (distLeft < distRight) {
            if (ball->getEsquerre()) nodeMesProper(targetQuery, Q, ball->getEsquerre());
            if (ball->getDreta())    nodeMesProper(targetQuery, Q, ball->getDreta());
        } else {
            if (ball->getDreta())    nodeMesProper(targetQuery, Q, ball->getDreta());
            if (ball->getEsquerre()) nodeMesProper(targetQuery, Q, ball->getEsquerre());
        }
    }
    return Q;
}