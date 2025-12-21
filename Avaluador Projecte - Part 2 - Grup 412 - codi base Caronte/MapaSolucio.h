// NEW PART 2
#pragma once
#include "pch.h"

#include "MapaBase.h"

// Pots esborrar tot aquest fitxer i utilitzar el teu si ho veus oportu.

class MapaSolucio : public MapaBase {

public:
    MapaSolucio() {

    }

    ~MapaSolucio() {

    }

    // Metodes a implementar de la primera part
    void getPdis(std::vector<PuntDeInteresBase*>&);
    void getCamins(std::vector<CamiBase*>&);
    void parsejaXmlElements(std::vector<XmlElement>& xmlElements);

    // Metode a implementar de la segona part
    CamiBase* buscaCamiMesCurt(PuntDeInteresBase* desde, PuntDeInteresBase* a);

private:
    // TODO: Afegeix els atributs que creguis necessaris per aquest MapaSolucio
};
