#ifndef _BALL_H
#define _BALL_H

#include "Util.h"
#include <vector>
#include <list>
#include <algorithm>
#include "pch.h" // Caronte

using namespace std;


// estructura ball tree para busquedas espaciales
class BallTree {
public:
	BallTree() { // constructor base
		m_left = nullptr;
		m_right = nullptr;
		m_root = nullptr;
		m_radi = 0.001f;
		m_pivot = Coordinate{ 0.0, 0.0 };
	}

	// Getters In-Line
	BallTree* getArrel() {
		// si no hay raiz, este nodo es la raiz
		if (m_root == nullptr)
			return this;
		return m_root;
	}

	Coordinate getPivot() { return m_pivot; }
	float getRadi() { return m_radi; }
	BallTree* getDreta() { return m_right; }
	BallTree* getEsquerre() { return m_left; }
	vector<Coordinate>& getCoordenades() { return m_coordenades; }

	// Setters In-Line
	void setArrel(BallTree* root) { m_root = root; }
	void setPivot(Coordinate pivot) { m_pivot = pivot; }
	void setRadius(float radi) { m_radi = radi; }
	void setDreta(BallTree* right) { m_right = right; }
	void setEsquerre(BallTree* left) { m_left = left; }
	void setCoordenades(vector<Coordinate>& coordenades) { m_coordenades = coordenades; }

	// busca el nodo mas cercano a un punto
	Coordinate nodeMesProper(Coordinate pdi, Coordinate& Q, BallTree* ball);

	// construccion del arbol
	void construirArbre(const vector<Coordinate>& coordenades);

	// recorridos clasicos (debug / visualizacion)
	void inOrdre(vector<list<Coordinate>>& out);
	void preOrdre(vector<list<Coordinate>>& out);
	void postOrdre(vector<list<Coordinate>>& out);

	// destructor (si o si o peta)
	~BallTree();

private:
	BallTree* m_root;              // raiz real del arbol
	BallTree* m_left;              // hijo izquierdo
	BallTree* m_right;             // hijo derecho
	
	float m_radi;                  // radio del nodo
	Coordinate m_pivot;             // punto central
	
	vector<Coordinate> m_coordenades; // puntos contenidos
};


/* --------------------------------------------------
 *  lp project - mapa / camins / pdis
 *  arnau baeza muñoz        niu: 1708086
 *  felipe tenorio da silva  niu: 1708283
 * --------------------------------------------------
 */

#endif
