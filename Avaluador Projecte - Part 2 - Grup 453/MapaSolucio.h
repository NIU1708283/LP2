#pragma once

#include "MapaBase.h"
#include "PuntDeInteresBotigaSolucio.h"
#include "PuntDeInteresRestaurantSolucio.h"
#include "CamiSolucio.h"
#include <vector>
#include "XML4OSMUtilModificat.h"

class MapaSolucio : public MapaBase {
private:
    std::vector<PuntDeInteresBase*> m_pdis;
    std::vector<CamiBase*> m_camins;

public:
    ~MapaSolucio(){}
    MapaSolucio() {}

    void getPdis(std::vector<PuntDeInteresBase*>& pdis){
        pdis = m_pdis;
        
    }

    void getCamins(std::vector<CamiBase*>& camins) override {
        camins = m_camins;
    }

    void parsejaXmlElements(std::vector<XmlElement>& xmlElements){
        m_pdis.clear();
        m_camins.clear();

        for (int i = 0; i < xmlElements.size(); i++)
        {
            bool name = false;
            bool highway = false;
            int camiInteres = -1;
            if (xmlElements[i].fills.size() == 0)
            {
                camiInteres = 0;
            }else{
                for (int h = 0; h < xmlElements[i].fills.size(); h++)//para el numero de tag
                {
                    if (xmlElements[i].id_element == "node")
                    {
                        for (int k = 0; k < xmlElements[i].fills[h].second.size(); k++)//para el subnumuero de tag
                        {
                            if (xmlElements[i].fills[h].second[k].second == "name")
                            {
                                name = true;
                            }

                            if (xmlElements[i].fills[h].second[k].second == "highway" || xmlElements[i].fills[h].second[k].second == "public_transport" || xmlElements[i].fills[h].second[k].second == "access" || xmlElements[i].fills[h].second[k].second == "entrance")
                            {
                                highway = true;
                            }
                            

                        }
                    }
                }
            }

            if (xmlElements[i].id_element == "way")
            {
                camiInteres = 0;
            }else if(name && !highway && xmlElements[i].fills.size() > 0)
            {
                camiInteres = 1;
            }
            
            

            double lat, lon;
            if (camiInteres == 0 && xmlElements[i].id_element == "way"){
                std::vector<Coordinate> coords;
                for (int h = 0; h < xmlElements[i].fills.size(); h++)//para el numero de tag
                {
                    for (int k = 0; k < xmlElements[i].fills[h].second.size(); k++)//para el subnumuero de tag
                    {
                        if (xmlElements[i].fills[h].second[k].first == "ref")
                        {
                            std::string id = xmlElements[i].fills[h].second[k].second;
                            coords.push_back(buscadorDeCordsByID(id, xmlElements));
                        }
                        
                    }
                }
                
                m_camins.push_back(new CamiSolucio(coords, getNameXML(xmlElements[i].fills)));
                
            }

            if (camiInteres == 1)
            {
                
                Coordinate cordenadas_pInteres = getCoordenadas(xmlElements[i].atributs);
                for (int h = 0; h < xmlElements[i].fills.size(); h++)//para el numero de tag
                {
                    for (int k = 0; k < xmlElements[i].fills[h].second.size(); k++)//para el subnumuero de tag
                    {
                        if (xmlElements[i].fills[h].second[k].second == "restaurant" || xmlElements[i].fills[h].second[k].second == "cafe")
                        {
                            m_pdis.push_back(new PuntDeInteresRestaurantSolucio(cordenadas_pInteres, getNameXML(xmlElements[i].fills), getTipusCuina(xmlElements[i].fills), getWheelChair(xmlElements[i].fills)));
                        }else if (xmlElements[i].fills[h].second[k].second == "shop")
                        {
                            m_pdis.push_back(new PuntDeInteresBotigaSolucio(cordenadas_pInteres, getNameXML(xmlElements[i].fills), getTipusShop(xmlElements[i].fills), getApertura(xmlElements[i].fills), getWheelChair(xmlElements[i].fills)));
                        }
                    }
                }
                
            }
        
        }
        
    }

    std::string getNameXML(const std::vector<CHILD_NODE>& children) const{
    for (int i = 0; i < children.size(); i++) {
        for (int j = 0; j < children[i].second.size(); j++) {
            // Verificamos si el atributo actual es "k" y tiene valor "name"
            if (children[i].second[j].first == "k" && children[i].second[j].second == "name") {
                // Si encontramos "name", retornamos el valor asociado en el siguiente elemento (v)
                return children[i].second[j + 1].second; 
            }
        }
    }
    return "noName";
    }

    Coordinate getCoordenadas(const std::vector<PAIR_ATTR_VALUE>& atributo) const {
        double lat, lon;
        Coordinate c;

        for (int i = 0; i < atributo.size(); i++)
        {
            if (atributo[i].first == "lat")
            {
                c.lat = std::stod(atributo[i].second);
            }else if (atributo[i].first == "lon")
            {
                c.lon = std::stod(atributo[i].second);
            }
        }
        
        return c;
        
    }

    bool getWheelChair(const std::vector<CHILD_NODE>& children) const {
        
        for (int i = 0; i < children.size(); i++) {
            for (int j = 0; j < children[i].second.size(); j++) {
                // Verificamos si el atributo actual es "k" y tiene valor "wheelchair"
                if (children[i].second[j].first == "k" && children[i].second[j].second == "wheelchair") {
                    // Comprobamos si el siguiente valor es "yes"
                    return children[i].second[j + 1].second == "yes";
                }
            }
        }
        return false;

    }

    std::string getTipusCuina(const std::vector<CHILD_NODE>& children) const {
        for (int i = 0; i < children.size(); i++) {
            for (int j = 0; j < children[i].second.size(); j++) {
                // Comprobamos si el atributo actual es "k" y tiene valor "cuisine"
                if (children[i].second[j].first == "k" && children[i].second[j].second == "cuisine") {
                    // Devolvemos el tipo de cocina (valor siguiente al "k" == "cuisine")
                    return children[i].second[j + 1].second;
                }
            }
        }
        return "noCuisine";

    }

    std::string getApertura(const std::vector<CHILD_NODE>& children) const {
        for (int i = 0; i < children.size(); i++) {
            for (int j = 0; j < children[i].second.size(); j++) {
                // Comprobamos si el atributo actual es "k" y tiene valor "opening_hours"
                if (children[i].second[j].first == "k" && children[i].second[j].second == "opening_hours") {
                    // Devolvemos el horario de apertura (valor siguiente al "k" == "opening_hours")
                    return children[i].second[j + 1].second;
                }
            }
        }
        return "noOpeningHours";
    }

    std::string getTipusShop(const std::vector<CHILD_NODE>& children) const {
        for (int i = 0; i < children.size(); i++) {
            for (int j = 0; j < children[i].second.size(); j++) {
                // Comprobamos si el atributo actual es "k" y tiene valor "shop"
                if (children[i].second[j].first == "k" && children[i].second[j].second == "shop") {
                    // Devolvemos el tipo de tienda (valor siguiente al "k" == "shop")
                    return children[i].second[j + 1].second;
                }
            }
        }
        return "noShopType";
    }

    Coordinate buscadorDeCordsByID(const std::string& id, const std::vector<XmlElement>& e) {
        for (int i = 0; i < e.size(); i++) {
            if (e[i].id_element == "node") {
                for (int j = 0; j < e[i].atributs.size(); j++) {
                    if (e[i].atributs[j].first == "id" && e[i].atributs[j].second == id) {
                        // Si se encuentra, devolver las coordenadas del nodo
                        return getCoordenadas(e[i].atributs);
                    }
                }
            }
        }

        // Si no se encuentra, devolver un Coordinate por defecto
        Coordinate cord;
        cord.lat = 0.0;
        cord.lon = 0.0;
        return cord;
    }

    CamiBase* buscaCamiMesCurt(PuntDeInteresBase* desde, PuntDeInteresBase* a) {
        std::vector<Coordinate> coords;
        CamiSolucio* cami = new CamiSolucio(coords, "Camino");
        return cami;
        //No se que fer
    }


};

