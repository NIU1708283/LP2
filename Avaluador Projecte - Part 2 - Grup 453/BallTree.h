#ifndef _BALL_H
#define _BALL_H

#include "Util.h"
#include <list>
#include <algorithm>

using namespace std;

class BallTree {

public:
	BallTree() {
		m_left = nullptr;
		m_right = nullptr;
		m_radi = 0.001;
		m_pivot = Coordinate { 0.0, 0.0 };
        m_root = nullptr;
    }

    // Getters
    BallTree* getArrel() {
	    return m_root;
	}

    Coordinate getPivot() {
		return m_pivot;
	}

	double getRadi() {
		return m_radi;
	}

	BallTree* getDreta() {
		return m_right;
	}

	BallTree* getEsquerre() {
		return m_left;
	}

	std::vector<Coordinate>& getCoordenades() {
		return m_coordenades;
	}

	// Setters
    void setArrel(BallTree* root) {
        m_root = root;
    }

	void setPivot(Coordinate pivot) {
		m_pivot = pivot;
	}

	void setRadius(double radi) {
		m_radi = radi;
	}

	void setDreta(BallTree* right) {
		m_right = right;
	}

	void setEsquerre(BallTree* left) {
		m_left = left;
	}

	void setCoordenades(std::vector<Coordinate>& coordenades) {
		m_coordenades = coordenades;
	}

    // Metodes a implementar
    void construirArbre(const std::vector<Coordinate>& coordenades) {
    if (!m_root) {
        m_root = this; // Apuntar al nodo raíz solo la primera vez
    }
    
    if (coordenades.empty()) return;

    // Asignar todas las coordenadas al nodo actual
    m_coordenades = coordenades;
    

    // Calcular el punto central (Punto C)
    m_pivot = Util::calcularPuntCentral(coordenades);

    // Calcular todas las distancias desde el Punto C
    std::vector<double> distanciasC;
    for (const auto& coord : coordenades) {
        distanciasC.push_back(Util::DistanciaHaversine(m_pivot, coord));
    }

    // Tomar el punto más lejano (Punto A)
    auto maxIt = max_element(distanciasC.begin(), distanciasC.end());
    Coordinate puntoA = coordenades[distance(distanciasC.begin(), maxIt)];

    // Calcular todas las distancias desde el Punto A
    std::vector<double> distanciasA;
    for (const auto& coord : coordenades) {
        distanciasA.push_back(Util::DistanciaHaversine(puntoA, coord));
    }

    // Tomar el punto más lejano (Punto B)
    auto maxItB = max_element(distanciasA.begin(), distanciasA.end());
    Coordinate puntoB = coordenades[distance(distanciasA.begin(), maxItB)];

    // Dividir los nodos en bolas izquierda y derecha
    std::vector<Coordinate> leftCoords, rightCoords;
    for (int i = 0; i < coordenades.size(); i++) {
        double D1 = Util::DistanciaHaversine(puntoA, coordenades[i]);
        double D2 = Util::DistanciaHaversine(puntoB, coordenades[i]);
        if (D1 < D2) {
            leftCoords.push_back(coordenades[i]);
        } else {
            rightCoords.push_back(coordenades[i]);
        }
    }

    // Asignar el radio como la distancia máxima desde el pivote
    m_radi = *std::max_element(distanciasC.begin(), distanciasC.end());

    // Caso base: Si el nodo actual contiene pocas coordenadas, no dividir más
    if (coordenades.size() == 1) {
        m_coordenades = coordenades;
        m_pivot = coordenades[0];
        m_radi = 0.0; 
        return;
    }

    // Crear los nodos hijos y construir los subárboles
    if (!leftCoords.empty()) {
        m_left = new BallTree();
        m_left->construirArbre(leftCoords);
    }
    if (!rightCoords.empty()) {
        m_right = new BallTree();
        m_right->construirArbre(rightCoords);
    }
}

    // void construirArbre(const std::vector<Coordinate>& coordenades){
    //     if (coordenades.empty()) return;

    //     // Calcular el punto central (Punto C)
    //     m_pivot = Util::calcularPuntCentral(coordenades);

    //     // Calcular todas las distancias desde el Punto C
    //     std::vector<double> distanciasC;
    //     for (int i = 0; i < coordenades.size(); i++) {
    //         distanciasC.push_back(Util::DistanciaHaversine(m_pivot, coordenades[i]));
    //     }
        
    //     // Tomar el punto más lejano (Punto A)
    //     auto maxIt = max_element(distanciasC.begin(), distanciasC.end());
    //     Coordinate puntoA = coordenades[distance(distanciasC.begin(), maxIt)];

    //     // Calcular todas las distancias desde el Punto A
    //     std::vector<double> distanciasA;
    //     for (int i = 0; i < coordenades.size(); i++) {
    //         distanciasA.push_back(Util::DistanciaHaversine(puntoA, coordenades[i]));
    //     }

    //     // Tomar el punto más lejano (Punto B)
    //     auto maxItB = max_element(distanciasA.begin(), distanciasA.end());
    //     Coordinate puntoB = coordenades[distance(distanciasA.begin(), maxItB)];

    //     // Dividir los nodos en bolas izquierda y derecha
    //     std::vector<Coordinate> leftCoords, rightCoords;
    //     for (const auto& coord : coordenades) {
    //         double D1 = Util::DistanciaHaversine(puntoA, coord);
    //         double D2 = Util::DistanciaHaversine(puntoB, coord);
    //         if (D1 < D2) {
    //             leftCoords.push_back(coord);
    //         } else {
    //             rightCoords.push_back(coord);
    //         }
    //     }

    //     // Asignar el radio como la distancia máxima desde el pivote
    //     m_radi = *std::max_element(distanciasC.begin(), distanciasC.end());

    //     if (coordenades.size() == 1) {
    //         m_coordenades = coordenades;
    //         m_pivot = coordenades[0];
    //         m_radi = 0.0;           
    //         return;
    //     }

    //     // Crear los nodos hijos y construir los subárboles
    //     if (!leftCoords.empty()) {
    //         m_left = new BallTree();
    //         m_left->construirArbre(leftCoords);
    //     }
    //     if (!rightCoords.empty()) {
    //         m_right = new BallTree();
    //         m_right->construirArbre(rightCoords);
    //     }
    // }

    void inOrdre(std::vector<std::list<Coordinate>>& out){
        if (m_left) m_left->inOrdre(out);
        out.push_back(std::list<Coordinate>(m_coordenades.begin(), m_coordenades.end()));
        if (m_right) m_right->inOrdre(out);
    }
    void preOrdre(std::vector<std::list<Coordinate>>& out){
        out.push_back(std::list<Coordinate>(m_coordenades.begin(), m_coordenades.end()));
        if (m_left) m_left->preOrdre(out);
        if (m_right) m_right->preOrdre(out);
    }
    void postOrdre(std::vector<std::list<Coordinate>>& out){
        if (m_left) m_left->postOrdre(out);
        if (m_right) m_right->postOrdre(out);
        out.push_back(std::list<Coordinate>(m_coordenades.begin(), m_coordenades.end()));
    }

    Coordinate nodeMesProper(Coordinate targetQuery, Coordinate& Q, BallTree* ball) {
        if (ball == nullptr) return Coordinate{ 0.0, 0.0 };

        // Si el nodo actual es una hoja, devolver el punto más cercano
        if (ball->m_left == nullptr && ball->m_right == nullptr) {
            return ball->m_pivot;
        }

        // Calcular la distancia entre el punto objetivo y el pivote del nodo actual
        double dist = Util::DistanciaHaversine(targetQuery, ball->m_pivot);

        // Si la distancia es menor que el radio, buscar en ambos subárboles
        if (dist < ball->m_radi) {
            Coordinate leftClosest = nodeMesProper(targetQuery, Q, ball->m_left);
            Coordinate rightClosest = nodeMesProper(targetQuery, Q, ball->m_right);

            // Calcular la distancia entre el punto objetivo y los pivotes de los subárboles
            double distLeft = Util::DistanciaHaversine(targetQuery, leftClosest);
            double distRight = Util::DistanciaHaversine(targetQuery, rightClosest);

            // Devolver el punto más cercano
            return distLeft < distRight ? leftClosest : rightClosest;
        }

        // Si la distancia es mayor que el radio, buscar en un solo subárbol
        if (Util::DistanciaHaversine(targetQuery, ball->m_left->m_pivot) < Util::DistanciaHaversine(targetQuery, ball->m_right->m_pivot)) {
            return nodeMesProper(targetQuery, Q, ball->m_left);
        } else {
            return nodeMesProper(targetQuery, Q, ball->m_right);
        }
    }
    // if (!ball) return Q;

    //     double D1 = Util::DistanciaHaversine(ball->getPivot(), targetQuery);
    //     double D2 = Util::DistanciaHaversine(ball->getPivot(), Q);

    //     if (D1 - ball->getRadi() >= D2) {
    //         return Q;
    //     }

    //     if (!ball->getDreta() && !ball->getEsquerre()) {
    //         for (const auto& coord : ball->getCoordenades()) {
    //             double dist = Util::DistanciaHaversine(coord, targetQuery);
    //             if (dist < D2) {
    //                 Q = coord;
    //                 D2 = dist;
    //             }
    //         }
    //         return Q;
    //     }

    //     double Da = ball->getEsquerre() ? Util::DistanciaHaversine(ball->getEsquerre()->getPivot(), targetQuery) : std::numeric_limits<double>::infinity();
    //     double Db = ball->getDreta() ? Util::DistanciaHaversine(ball->getDreta()->getPivot(), targetQuery) : std::numeric_limits<double>::infinity();

    //     if (Da < Db) {
    //         Q = nodeMesProper(targetQuery, Q, ball->getEsquerre());
    //         Q = nodeMesProper(targetQuery, Q, ball->getDreta());
    //     } else {
    //         Q = nodeMesProper(targetQuery, Q, ball->getDreta());
    //         Q = nodeMesProper(targetQuery, Q, ball->getEsquerre());
    //     }

    //     return Q;
    // Destructor
    ~BallTree() = default;

private:
    BallTree* m_root;
    BallTree* m_left;
	BallTree* m_right;
	double m_radi;
	Coordinate m_pivot;
	std::vector<Coordinate> m_coordenades;

};


#endif