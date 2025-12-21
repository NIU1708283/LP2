#include "MapaSolucio.h"
#include "GrafSolucio.h"
#include "BallTree.h"

void MapaSolucio::getPdis(vector<PuntDeInteresBase*>& pids)
{
	pids.clear();
	for (auto it = m_pids.begin(); it != m_pids.end(); it++)
		pids.push_back(*it);
}

void MapaSolucio::getCamins(vector<CamiBase*>& camins)
{
	camins.clear();
	for (auto it = m_camins.begin(); it != m_camins.end(); it++)
		if ((*it)->getHighway() == true)   //Només els highway, encara que no afecta a la nota del projecte
			camins.push_back(*it);
}

void MapaSolucio::parsejaXmlElements(vector<XmlElement>& xmlElements)
{
	//Borramos las listas
	for (auto it = m_pids.begin(); it != m_pids.end(); it++)
		delete (*it);
	m_pids.clear();
	for (auto it = m_camins.begin(); it != m_camins.end(); it++)
		delete (*it);
	m_camins.clear();
	vector<pair<string, Coordinate>> refs; //Vector donde guardaremos los no punto de interes.

	for (auto it = xmlElements.begin(); it != xmlElements.end(); it++)
	{ //Recorremos la lista de xmlElements

		float lat = 0, lon = 0;
		if ((*it).id_element == "node")
		{ //Distingir si el node es un restaurant o una botiga
			int type = 0;

			for (int i = 0; i < (*it).fills.size(); i++)
			{ //Buscamos dentro de los hijos el tag
				if ((*it).fills[i].first == "tag")
				{
					pair<string, string> valorTag = Util::kvDeTag((*it).fills[i].second);
					if (valorTag.first == "shop")
						type = 1;
					else if (valorTag.second == "restaurant" || valorTag.second == "cafe")
						type = 2;
				}
			}
			//Dependiendo del tipo de punto de interes, rellenamos los datos
			switch (type)
			{
			case 1: // En caso de ser una botiga
			{
				Coordinate c;
				string name, shop, wheelchair, opening_hours;

				for (int i = 0; i < (*it).atributs.size(); i++)
				{ //De los atributos buscamos las cordenadas
					if ((*it).atributs[i].first == "lat")
						lat = stod((*it).atributs[i].second); // Convertimos el string a float
					if ((*it).atributs[i].first == "lon")
						lon = stod((*it).atributs[i].second);
					c.lat = lat;
					c.lon = lon;
				}

				for (int i = 0; i < (*it).fills.size(); i++)
				{
					if ((*it).fills[i].first == "tag")
					{ //De los hijos (tags) buscamos toda la informacion para rellenar el punto
						pair<string, string> valorTag = Util::kvDeTag((*it).fills[i].second);

						if (valorTag.first == "name")
							name = valorTag.second;
						else if (valorTag.first == "shop")
							shop = valorTag.second;
						else if (valorTag.first == "wheelchair")
							wheelchair = valorTag.second;
						else if (valorTag.first == "opening_hours")
							opening_hours = valorTag.second;
					}
				}

				if (name != "") //Si el nombre no esta vacio, lo añadimos a la lista
					m_pids.push_back(new PuntDeInteresBotigaSolucio(c, name, shop, opening_hours, wheelchair));

				break;
			}
			case 2: //En caso de ser un restaurante
			{
				Coordinate c;
				string name, cuisine, wheelchair;
				for (int i = 0; i < (*it).atributs.size(); i++)
				{ //De los atributos buscamos las cordenadas
					if ((*it).atributs[i].first == "lat")
						lat = stod((*it).atributs[i].second); //Convertimos el string a float
					if ((*it).atributs[i].first == "lon")
						lon = stod((*it).atributs[i].second);

					c.lat = lat;
					c.lon = lon;
				}

				for (int i = 0; i < (*it).fills.size(); i++)
				{ //De los hijos (tags) buscamos toda la informacion para rellenar el punto
					if ((*it).fills[i].first == "tag")
					{
						// Emmagatzemem el valor d'aquest tag
						pair<string, string> valorTag = Util::kvDeTag((*it).fills[i].second); //Separamos el tag en clave y valor
						// Comprovem que es el tag que busquem
						if (valorTag.first == "name")
							name = valorTag.second;
						else if (valorTag.first == "cuisine")
							cuisine = valorTag.second;
						else if (valorTag.first == "wheelchair")
							wheelchair = valorTag.second;
					}

				}

				if (name != "") //Si el nombre no esta vacio, lo añadimos a la lista
					m_pids.push_back(new PuntDeInteresRestaurantSolucio(c, name, cuisine, wheelchair));

				break;
			}
			default: //En caso de no ser ninguno de los dos 

				//En cas de no ser un Punt de Interes
				Coordinate c;
				string id;
				for (int i = 0; i < (*it).atributs.size(); i++)
				{//De los atributos buscamos las cordenadas i el id
					if ((*it).atributs[i].first == "id")
						id = (*it).atributs[i].second;
					if ((*it).atributs[i].first == "lat")
						lat = stod((*it).atributs[i].second);
					if ((*it).atributs[i].first == "lon")
						lon = stod((*it).atributs[i].second);
					c.lat = lat;
					c.lon = lon;
				}

				bool isIn = false; //Comprobamos si ya esta en la lista
				for (auto it2 = refs.begin(); it2 != refs.end(); it2++)
					if ((*it2).first == id)
						isIn = true;

				if (!isIn) //Si no esta en la lista, lo añadimos
				{
					pair<string, Coordinate> ref;
					ref.first = id;
					ref.second = c;
					refs.push_back(ref);
				}
				break;
			}
		}

		bool isHighway = false;
		if ((*it).id_element == "way")
		{ //En caso de no ser punto, miramos si es camino
			vector<Coordinate> resultat; //Vector donde guardaremos las cordenadas
			for (int i = 0; i < (*it).fills.size(); i++)
			{
				if ((*it).fills[i].first == "tag") //Buscamos el tag
				{
					pair<string, string> valorTag = Util::kvDeTag((*it).fills[i].second); //Separamos el tag en clave y valor
					if (valorTag.first == "highway")
						isHighway = true;
				}

				if ((*it).fills[i].first == "nd") //Buscamos las referencias
				{
					for (auto it2 = refs.begin(); it2 != refs.end(); it2++)
						if ((*it).fills[i].second[0].second == (*it2).first)
							resultat.push_back((*it2).second);
				}
			}
			if (isHighway) //Si es un camino, lo añadimos a la lista
				m_camins.push_back(new CamiSolucio(resultat, isHighway));
		}
	}
}

	// Segona part
	//Aquesta funció retorna el camí més curt entre dos punts de la llista de punts d'interès
	CamiBase* MapaSolucio::buscaCamiMesCurt(PuntDeInteresBase* desde, PuntDeInteresBase* a)
	{
		GrafSolucio graf(this); //Creem el graf a partir del mapa actual
		BallTree ball; //Creamos el balltree
		ball.construirArbre(graf.getCoordenades()); //Construimos el arbol a partir del grafo actual

		//Fem crida reiterativa de cada una de les boles per trobar el node mes proper a unes cordenades
		Coordinate inici, final; //Puntos de inicio y final
		ball.nodeMesProper(desde->getCoord(), inici, &ball); //Buscamos el nodo mas cercano al inicio
		ball.nodeMesProper(a->getCoord(), final, &ball); //Buscamos el nodo mas cercano al final

		vector<Coordinate> qCami; //Vector de coordenadas del camino
		stack<Coordinate> pilaQCami; //Pila de coordenadas del camino
		graf.camiMesCurt(inici, final, pilaQCami); //Buscamos el camino mas corto entre los dos puntos (con dijkstra)

		//Hay que "girar" la pila por lo que hacemos pops y lo añadimos al vector, actualmente: final->inicio y queremos inicio->final
		while (!pilaQCami.empty()) //Mientras la pila no este vacia
		{
			qCami.push_back(pilaQCami.top()); //Añadimos el punto al vector
			pilaQCami.pop(); //Sacamos el punto de la pila
		}
		CamiBase* cami = new CamiSolucio(qCami, false); //Creamos el camino con las coordenadas
		return cami; //Devolvemos el camino
	}

	MapaSolucio::~MapaSolucio() {
		// Liberar memoria de los elementos en m_pids
		for (auto it = m_pids.begin(); it != m_pids.end(); ++it) {
			delete (*it);  // Liberar cada objeto apuntado
		}
		m_pids.clear(); // Vaciar el vector

		// Liberar memoria de los elementos en m_camins
		for (auto it = m_camins.begin(); it != m_camins.end(); ++it) {
			delete (*it);  // Liberar cada objeto apuntado
		}
		m_camins.clear(); // Vaciar el vector
	}