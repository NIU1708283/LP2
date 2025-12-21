#pragma once

#include "pch.h" // Caronte
#include <functional>
#include <string>
#include <vector>

using namespace std;

// pares k=v de los tags xml
typedef pair<string, string> PAIR_ATTR_VALUE;

// nodo hijo con sus atributos
typedef pair<string, vector<PAIR_ATTR_VALUE>> CHILD_NODE;


// coordenada simple lat/lon
typedef struct {
    double lat;
    double lon;
} Coordinate;


// puente para pasar pois a c / ui
extern "C" typedef struct {
    int i;
    double lat;
    double lon;
    unsigned int color;
    const char* title;
} PoiBridge;


// puente para pasar caminos
extern "C" typedef struct {
    double* lats;
    double* lons;
    int size;
} WayBridge;


// elemento xml parseado
typedef struct {
	string id_element;                 // node, way, etc
	vector<PAIR_ATTR_VALUE> atributs;  // atributos del elemento
	vector<CHILD_NODE> fills;          // hijos (tags, nd...)
} XmlElement;

// ## hasta aqui funciona


/* --------------------------------------------------
 *  lp project - mapa / camins / pdis
 *  arnau baeza muñoz        niu: 1708086
 *  felipe tenorio da silva  niu: 1708283
 * --------------------------------------------------
 */
