#ifndef _COMMON_H
#define _COMMON_H

#include <functional>
#include <string>
#include <vector>

typedef std::pair<std::string, std::string> PAIR_ATTR_VALUE;
typedef std::pair<std::string, std::vector<PAIR_ATTR_VALUE>> CHILD_NODE;

class Coordinate // creamos una clase para poder sobrecargar el operador == para poder comparar objetos de tipo Coordinate
{
public:
    float lat;
    float lon;
    bool operator==(const Coordinate& c) { return ((lat == c.lat) && (lon == c.lon)); }  //Per trobar el node al constructor del graf
};

extern "C" typedef struct {
    int i;
    float lat;
    float lon;
    unsigned int color;
    const char* title;
} PoiBridge;

extern "C" typedef struct {
    float* lats;
    float* lons;
    int size;
} WayBridge;

typedef struct {
    std::string id_element;
    std::vector<PAIR_ATTR_VALUE> atributs;
    std::vector<CHILD_NODE> fills;
} XmlElement;


#endif