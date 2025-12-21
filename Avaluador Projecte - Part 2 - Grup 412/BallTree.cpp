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

Coordinate BallTree::nodeMesProper(Coordinate targetQuery, Coordinate& Q, BallTree* ball) {
    if (!ball) return Q;

    // Si Q és (0,0), l'inicialitzem amb el primer punt vàlid de la bola actual
    // Això assegura que sempre tinguem una distància real per comparar.
    if (Q.lat == 0 && Q.lon == 0 && !ball->getCoordenades().empty()) {
         Q = ball->getCoordenades()[0];
    }
    
    // Si hem trobat exactament el punt, retornem
    if (targetQuery.lat == Q.lat && targetQuery.lon == Q.lon) return Q;

    // Distàncies clau
    double distPdiPivot = Util::DistanciaHaversine(targetQuery, ball->getPivot());
    double distPdiQ = Util::DistanciaHaversine(targetQuery, Q);

    // Poda triangular: Si la bola sencera està més lluny que el millor punt trobat (Q), l'ignorem.
    // (distancia al centre - radi) és la distància mínima possible a qualsevol punt de la bola.
    if (distPdiPivot - ball->getRadi() >= distPdiQ) {
        return Q;
    }

    // Cas FULLA: Iterem per TOTS els punts de la llista (com fa el grup 431)
    if (!ball->getEsquerre() && !ball->getDreta()) {
        for (const auto& punt : ball->getCoordenades()) {
            double d = Util::DistanciaHaversine(targetQuery, punt);
            // Important: Recalcular la distància a Q perquè Q pot haver canviat
            if (d < Util::DistanciaHaversine(targetQuery, Q)) {
                Q = punt;
            }
        }
    } else {
        // Cas NODE INTERN: Ordenem la cerca visitant primer el fill més prometedor
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