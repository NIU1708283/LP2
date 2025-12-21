#ifndef _BALL_H
#define _BALL_H

#include "Util.h"
#include <list>
#include <algorithm>
using namespace std;

class BallTree 
{
public:
	BallTree() { m_left = nullptr; m_right = nullptr; m_radi = 0.001; m_pivot = Coordinate{ 0.0, 0.0 }; m_root = nullptr;}

	// Getters
	BallTree* getArrel() 
	{
		if (m_root == nullptr)
			return this;
		return m_root;
	}
	Coordinate getPivot() { return m_pivot; }
	float getRadi() { return m_radi; }
	BallTree* getDreta() { return m_right; }
	BallTree* getEsquerre() { return m_left; }
	vector<Coordinate>& getCoordenades() { return m_coordenades; }

	// Setters
	void setArrel(BallTree* root) { m_root = root; }
	void setPivot(Coordinate pivot) { m_pivot = pivot; }
	void setRadius(float radi) { m_radi = radi; }
	void setDreta(BallTree* right) { m_right = right; }
	void setEsquerre(BallTree* left) { m_left = left; }
	void setCoordenades(vector<Coordinate>& coordenades) { m_coordenades = coordenades; }

	Coordinate nodeMesProper(Coordinate pdi, Coordinate& Q, BallTree* ball); //Devuelve las coordenadas del nodo más cercano

	// Metodes a implementar
	void construirArbre(const vector<Coordinate>& coordenades); //Construye el arbol
	void inOrdre(vector<list<Coordinate>>& out);
	void preOrdre(vector<list<Coordinate>>& out);
	void postOrdre(vector<list<Coordinate>>& out);

	// Destructor
	~BallTree();

private:
	BallTree* m_root;
	BallTree* m_left;
	BallTree* m_right;
	float m_radi;
	Coordinate m_pivot;
	vector<Coordinate> m_coordenades;

};


#endif
