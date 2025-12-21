#include "BallTree.h"
#include <limits> //per utilitzar DBL_MAX
#include <stack> // per utilitzar la pila de la funció nodeMesProper
#include <cfloat>

//Q és la coordenada del arbre del qual és la distància més curta respecte pdi.
Coordinate BallTree::nodeMesProper(Coordinate pdi, Coordinate& Q, BallTree* ball)
{
    if (pdi.lon == Q.lon && pdi.lat == Q.lat) // si es el valor que busquem ja esta
        return Q;

    if (ball->getArrel() == nullptr) // si el node no té pare les cordenadas seran 0.0 (forçar que l'algorisme actualitzi)
    {
        Q.lat = 0;
        Q.lon = 0;
    }

    //PAS 1
    float D1 = Util::DistanciaHaversine(pdi, ball->m_pivot); // calculem la distancia entre el pdi y el pivot
    //PAS 2
    float D2 = Util::DistanciaHaversine(Q, ball->m_pivot); // calculem la distancia entre Q y el pivot
    //PAS 3
    if ((D1 - ball->m_radi) >= D2)  //Si D1 – bola.radi >= D2,retorna Q
        return Q;
    else
    {
        //PAS 4
        if (ball->m_left == nullptr && ball->m_right == nullptr) // Si la bola és una fulla de l’arbre = No te fills
        {
            // actualitza Q si és el node camí més proper al punt d’interès, dels punts que formen la bola
            for (auto it = m_coordenades.begin(); it != m_coordenades.end(); it++) // per a cada coordenada
            {
                if (Util::DistanciaHaversine(pdi, *it) < Util::DistanciaHaversine(pdi, Q)) // Si el node acutal esta més a prop de pid que Q, actualitzem el node Q
                {
                    Q.lat = (*it).lat;
                    Q.lon = (*it).lon;
                }
            }
        }
        else
        {
            //PAS 5
            float Da = DBL_MAX;
            float Db = DBL_MAX;
            //Calculem les distàncies del pdi(Punt d'interès) respecte el pivot per les pilotes fills. (Pel fill o fills que tingui)
            if (ball->m_right != nullptr) //Si té fill dret calculem la distància
                Db = Util::DistanciaHaversine(pdi, ball->m_right->m_pivot);
            if (ball->m_left != nullptr) //Si té fill esquerra calculem la distància
                Da = Util::DistanciaHaversine(pdi, ball->m_left->m_pivot);

            if (Da < Db) //Si Da < Db, comença la cerca per la pilota esquerra, i després, la dreta
            {
                if (ball->m_left != nullptr)
                    nodeMesProper(pdi, Q, ball->m_left); // si el node té fill esquerra, trucada recursiva
                if (ball->m_right != nullptr)
                    nodeMesProper(pdi, Q, ball->m_right); // si el nodo tiene hijo derecho, trucada recursiva
            }
            else //Si Da > Db, comença la cerca per la pilota dreta, i després, la esquerra (Mirar si hi ha alguna Q més propera)
            {
                if (ball->m_right != nullptr)
                    nodeMesProper(pdi, Q, ball->m_right);
                if (ball->m_left != nullptr)
                    nodeMesProper(pdi, Q, ball->m_left);
            }
        }
        return Q;
    }
}

void BallTree::construirArbre(const vector<Coordinate>& coordenades)
{
    m_coordenades = coordenades;
    m_pivot = Util::calcularPuntCentral(m_coordenades);  //Buscamos el pivote central
    vector<float> distanciesC;
    size_t posPuntMesLlunyC = 0;
   
    if (m_coordenades.size() > 1) //Si la bola només té una cordenada, fi = Condició de parada
    {
        //CALCULEM EL RADI/CORDENADA MÉS ALLUNYADA
        for (size_t i = 0; i < m_coordenades.size(); i++) // per la cada coordenada
        {
            distanciesC.push_back(Util::DistanciaHaversine(m_pivot, m_coordenades[i])); //Guardem en el vector la distància de cada cordenada al pivot
            if (distanciesC[posPuntMesLlunyC] < distanciesC[i])                         //Si la distància actual es més gran que la que guardem actualment, canviem
                posPuntMesLlunyC = i; // guardem la posició de la coordenada més allunyada
        }
        m_radi = distanciesC[posPuntMesLlunyC]; //El radi és la distància al punt més allunyat

        //BUSQUEM EL PUNT MÉS ALLUNYAT DEL PUNT MÉS LLUNYÀ
        Coordinate puntA;
        puntA.lat = m_coordenades[posPuntMesLlunyC].lat;
        puntA.lon = m_coordenades[posPuntMesLlunyC].lon;
        Coordinate puntB;   //Punt "Auxiliar" amb el que recorrerem tot el vector buscant el més allunyat (referencia inicial)
        puntB.lat = m_coordenades[0].lat;
        puntB.lon = m_coordenades[0].lon;
        Coordinate aux;
		for (size_t i = 1; i < m_coordenades.size(); i++) // per cada coordenada
        {
            aux.lat = m_coordenades[i].lat;
            aux.lon = m_coordenades[i].lon;
            if (Util::DistanciaHaversine(puntB, puntA) < Util::DistanciaHaversine(aux, puntA)) //Si es troba un punt aux que estigui més allunyat que el puntB actual, actualitzem puntB
            {
                puntB.lon = aux.lon;
                puntB.lat = aux.lat;
            }
        }

        //DIVIDIR LES CORDENADAS/NODES MÉS PROPERS A CADA FILL EN DOS VECTORS 
		vector<Coordinate> coordenadesA, coordenadesB; 
		for (size_t i = 0; i < m_coordenades.size(); i++) // per cada cordenada
        {
            if (Util::DistanciaHaversine(m_coordenades[i], puntA) < Util::DistanciaHaversine(m_coordenades[i], puntB)) //Si esta més a prop del puntA l'afegim al seu vector
                coordenadesA.push_back(m_coordenades[i]);
            else                                             //Si no, se l'afegim al puntB
                coordenadesB.push_back(m_coordenades[i]);
        }

        //Fem els dos arbres fill
        BallTree* fillDret = new BallTree;
        BallTree* fillEsquerre = new BallTree;
        //Els assignem com arrel/pare el arbre actual
        fillDret->m_root = this;
        fillEsquerre->m_root = this;
        //A l'arbre actual li assignem els seus fills
        m_left = fillEsquerre;
        m_right = fillDret;
        //De manera recursiva construim l'arbre amb cada un del seus fills, cada un amb els punts que tenen més a prop
        fillDret->construirArbre(coordenadesB);
        fillEsquerre->construirArbre(coordenadesA); 
    }
    else 
    { //Si només hi ha una cordenada...
        m_left = nullptr;
        m_right = nullptr;
        m_radi = 0;
    }
}

void BallTree::inOrdre(vector<list<Coordinate>>& out)
{
    if (m_coordenades.empty()) return; //Si esta buit no cal fer-lo
    //Com es inOrdre primer es fara el fill esquerra, despres es fara la llista y per ultim el fill dret
    if (m_left != nullptr) 
        m_left->inOrdre(out); 

	list<Coordinate> Cordenades(m_coordenades.begin(), m_coordenades.end()); // Fem una lista amb totes les cordenades actuals
	out.push_back(Cordenades); // afegim la lista de coordenadas al vector de llistes

    if (m_right != nullptr)
        m_right->inOrdre(out);
}

void BallTree::preOrdre(vector<list<Coordinate>>& out)
{
    if (m_coordenades.empty()) return; //Si esta buit no cal fer-lo
    //Como es inOrdre primer es fara la llista, despres es fara el fill esquerra y per ultim el fill dret
    list<Coordinate> Cordenades(m_coordenades.begin(), m_coordenades.end()); 
    out.push_back(Cordenades);

    if (m_left != nullptr)
        m_left->preOrdre(out);

    if (m_right != nullptr)
        m_right->preOrdre(out);
}

void BallTree::postOrdre(vector<list<Coordinate>>& out)
{
    if (m_coordenades.empty()) return; //Si esta buit no cal fer-lo
    //Com es inOrdre primer es fara el fill dret, despres es fara el fill esquerra y per ultim la llist
    if (m_left != nullptr)
        m_left->postOrdre(out);

    if (m_right != nullptr)
        m_right->postOrdre(out);

    list<Coordinate> Cordenades(m_coordenades.begin(), m_coordenades.end()); 
    out.push_back(Cordenades);
}

BallTree::~BallTree()
{
    // Liberar memoria dinámica de los hijos
    if (m_left != nullptr)
    {
        delete m_left;
        m_left = nullptr;
    }
    if (m_right != nullptr)
    {
        delete m_right;
        m_right = nullptr;
    }
}
