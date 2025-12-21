#include "pch.h"
#include "GrafSolucio.h"
#include <cfloat>

bool GrafSolucio::estaEnVector(const vector<Coordinate>& nodes, const Coordinate& c) //Funció auxiliar per saber si un node ja està al vector
{
	int i = 0;
	while (i != nodes.size())
	{
		if (nodes[i].lat == c.lat && nodes[i].lon == c.lon)
			return true;
		else
			i++;
	}
	return false;
}

//Destructor de la clase
GrafSolucio::~GrafSolucio()
{
	// Lliurar la memoria de m_matriuAdj si es necessari.
	for (size_t i = 0; i < m_matriuAdj.size(); i++) {
		m_matriuAdj[i].clear(); // Lliurar memoria de cada fila
	}
	m_matriuAdj.clear(); // Lliurar la matriu completa

	m_nodes.clear(); // Lliurar nodes
}

//Metode que s'encarrega de a partir d'un mapa agafar els camins que es trobin en ell y afegir els nodes, y aristes, al graf
GrafSolucio::GrafSolucio(MapaBase* map)
{
	m_nArestes = 0;
    m_nNodes = 0;
	vector<CamiBase*> camins;
	map->getCamins(camins); //Obtenim els camins del mapa (per referencia)

	for (int i = 0; i < camins.size(); i++)
	{
		vector<Coordinate> aux = camins[i]->getCamiCoords(); //Obtenim les coordenades del camí
		for (int i = 0; i < aux.size(); i++)
		{
			setNode(aux[i]); //afegim el node al graf
			if (i > 0) //Si només hi ha un node no cal cercar ni afegir aresta
			{
				size_t pos1 = 0, pos2=0;
				while (pos1 <= m_nodes.size()) //Busquem la posició dels nodes al vector de nodes
				{
					if (m_nodes[pos1] == aux[i])
						break;
					else
						pos1++;
				}
				while (pos2 <= m_nodes.size())
				{
					if (m_nodes[pos2] == aux[i - 1])
						break;
					else
						pos2++;
				}
				setAresta(pos1, pos2, Util::DistanciaHaversine(aux[i], aux[i - 1])); // inserim l'aresta amb el pes corresponent (funcio del Util)
			}
		}
	}
}

//Metode encarregat de crer un graf a partir de un vector de nodes i una matriu
GrafSolucio::GrafSolucio(const vector<Coordinate>& nodes, const vector<vector<float>>& matriuAdj)
{
	m_nodes = nodes;
	m_matriuAdj = matriuAdj;
	m_nArestes = 0;
	m_nNodes = nodes.size();
	for (int i = 0; i < m_nNodes; i++) 
	{
		for (int j = i + 1; j < m_nNodes; j++)
		{
			if (m_matriuAdj[i][j] != 0)
				m_nArestes++;
		}
	}
}

//Metode encarregat de crer un graf a partir de un vector de nodes, un vector de parelles de node i els pesos
GrafSolucio::GrafSolucio(const vector<Coordinate>& nodes, const vector<vector<float>>& parelles_nodes, const vector<float>& pesos)
{
	m_nodes = nodes;
	m_nNodes = m_nodes.size();
	m_nArestes = parelles_nodes.size();
	crearMatriu(parelles_nodes, pesos);
}

//Metode per inserir arestes
void GrafSolucio::setAresta(int posN1, int posN2, float pes)
{
	m_matriuAdj[posN1][posN2] = pes;
	m_matriuAdj[posN2][posN1] = pes;
}

//Metode per afegir nodes
void GrafSolucio::setNode(const Coordinate& n)
{
	if (!estaEnVector(m_nodes, n)) //Cal comprobar si el node ja esta a dins (per evitar repeticions)
	{
		m_nodes.push_back(n);	//Els afegim al vector de nodes
		m_matriuAdj.push_back(vector<float>(m_nNodes)); //Añadimos una nueba fila mas a la matriz
		m_nNodes++;
		for (int i = 0; i < m_nNodes; i++)
			m_matriuAdj[i].push_back(0);	//A cada nodo le añadimos un 0 al final de esta forma hemos añido una fila y una columna 
	}
}

//Metode que crea una matriu a partir de un vector de parelles i un vector de pesos
void GrafSolucio::crearMatriu(const vector<vector<float>>& parelles, const vector<float>& pesos)
{
	m_matriuAdj.resize(m_nNodes); //Creem una matriu de m_numNodes columnes
	for (size_t i = 0; i < m_nNodes; i++)
		m_matriuAdj[i].resize(m_nNodes, 0);  //Fem una matriu de m_numNodes files
	
	for (size_t i = 0; i < parelles.size(); i++)	//A cada parella li afegim el seu pes
	{
		m_matriuAdj[parelles[i][0]][parelles[i][1]] = pesos[i];	
		m_matriuAdj[parelles[i][1]][parelles[i][0]] = pesos[i];
	}
}

//Metode que calcula la distancia minima
size_t GrafSolucio::minDist(vector<float>& dist, vector<bool>& visitat)
{
	float min = DBL_MAX;	//Valor maxim
	size_t minIndex = -1;
	for (size_t posVei = 0; posVei < m_nNodes; posVei++) //para cada nodo
		if (!visitat[posVei] && dist[posVei] <= min) //si no ha sigut visitat y la distancia es menor que la minima
		{
			min = dist[posVei];	//Actualitzem valor
			minIndex = posVei; //actualitzem la minima
		}
	return minIndex;
}

//Metode que calcula el cami mes curt entre dos nodes
void GrafSolucio::camiMesCurt(const Coordinate& n1, const Coordinate& n2, stack<Coordinate>& cami)
{
	vector<Coordinate>::iterator itN1 = find(m_nodes.begin(), m_nodes.end(), n1); // busquem els nodes en el vector de nodes
	vector<Coordinate>::iterator itN2 = find(m_nodes.begin(), m_nodes.end(), n2);

	if ((itN1 != m_nodes.end()) && (itN2 != m_nodes.end())) // si els nodes existeixen
	{
		vector<float> vDist; //Vector de pesos
		size_t pos1 = distance(m_nodes.begin(), itN1); // obtenim la posicio dels nodes
		size_t pos2 = distance(m_nodes.begin(), itN2);
		vector<size_t> ant;
		dijkstra(pos1, pos2, vDist, ant); // apliquem dijkstra per conseguir el camin
		size_t it = pos2;
		cami.push(m_nodes[pos2]); // afegim el node al cami
		while (it != pos1) // mentre no arribem al node d'origen, seguim afegint nodes al cami retrocedint pel vector (ja que cada posicio guarda el node anterior en el cami)
		{
			cami.push(m_nodes[ant[it]]);
			it = ant[it];
		}
	}
}

//Metode que fa un recorregut dijkstra. Amb el cual conseguim el cami amb menys cost  (Inspiracio de las diapositives)
void GrafSolucio::dijkstra(size_t n1, size_t n2, vector<float>& dist, vector<size_t>& ant)
{
	vector<bool> visitat; //vector de visitats
	visitat.resize(m_nNodes, false); //Inicialitzem a false tot
	dist.resize(m_nNodes, DBL_MAX); //Inicialitzem les distancies a 'infinit' (valor mes gran posible)
	ant.resize(m_nNodes, -1);   //Vector que guarda per cada node, cual es el seu node anterior. D'aquesta manera podem recorrer cap enrere cuan arribem al node, y aixi conseguir el cami.
	dist[n1] = 0; //la distancia del node origen es 0
	ant[n1] = n1;	//Afegim al cami el vector actual.

	//Recorrem tots els nodes
	for (size_t count = 0; count < m_nNodes - 1; count++) 
	{
		size_t posVeiAct = minDist(dist, visitat);	//Busquem el node amb distancia minima y que no hagi estat visitat (per fer-ho pasem el vector de visitats)
		visitat[posVeiAct] = true;						//El node escollit ja el contem como visitat
		if (posVeiAct == n2) 
			return;					//Si ja hem arribat al node, finalitzem
		for (size_t posVei = 0; posVei < m_nNodes; posVei++)	//En cas de que no sigui el ultim, mirem els veins del node
		{
			if (m_matriuAdj[posVeiAct][posVei] != 0)
			{
				if (!visitat[posVei])	//Si la distancia al node actual + la distancia al node seguent es més petita que la distancia actual del seguent node vei, la canviem
				{
					if (dist[posVeiAct] + m_matriuAdj[posVeiAct][posVei] < dist[posVei]) //si la distancia actual es menor que la que ja teniem
					{
						dist[posVei] = dist[posVeiAct] + m_matriuAdj[posVeiAct][posVei]; 
						ant[posVei] = posVeiAct;		//Guardem l'anterior node del vei, el node actual (perque es el nou cami amb menys pes)
					}
				}
			}
		}
	}
}


