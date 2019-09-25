#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>        // std::abs
#include <list>
#include <algorithm>    // std::find
#include <vector>
#include <queue>

void ComportamientoJugador::PintaPlan(list<Action> plan)
{
	auto it = plan.begin();
	while (it!=plan.end())
	{
		if (*it == actFORWARD)
		{
			cout << "A ";
		}
		else if (*it == actTURN_R)
		{
			cout << "D ";
		}
		else if (*it == actTURN_L)
		{
			cout << "I ";
		}
		else
		{
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}

bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan)
{	
	cout << endl;
	cout << "******" << endl;
	cout << "Origen fil: " << origen.fila << "  col: " << origen.columna << endl;
	cout << "Destino fil: " << destino.fila << "  col: " << destino.columna << endl;
	cout << "******" << endl;

	cout << "actFORWARD: " << actFORWARD << endl;
	cout << "actTURN_L: " << actTURN_L << endl;
	cout << "actTURN_R: " << actTURN_R << endl;
	cout << "actIDLE: " << actIDLE << endl;

	list<Nodo> abiertos, cerrados, vecinos;
	char vecinoIzq, vecinoDcha, vecinoFrente;

	// Borro la lista del planes
	plan.clear();

	int costeH = distanciaManhattan(origen, destino);

	// Estado inicial
	Nodo inicio(origen, 0, costeH, costeH);  // el valor de f es 0 + costeH

	list<Action> ruta;

	cout << "Soy el nodo inicio: fil: " << inicio.getFil() << " col: " << inicio.getCol() << endl;

	// Añadir inicio a abiertos
	abiertos.push_back(inicio);

	while(!abiertos.empty())
	{
		cout << endl;
		cout << "**********" << endl;
		cout << "entro" << endl;
		cout << "**********" << endl;
		cout << endl;


		Nodo actual,aux;
		int min = 999999;

		list<Action> ruta_parcial;

		for(auto it = abiertos.begin(); it != abiertos.end(); ++it)
		{
			cout << "entro en el forrrr " << endl;
			if((*it).getCosteF() < min)
			{
				cout << "menor antes: " << min << endl;
				min = (*it).getCosteF();

				// asignamos nodo actual
				actual.setEstado((*it).getEstado());
				actual.setCosteG((*it).getCosteG());
				actual.setCosteH((*it).getCosteH());
				ruta_parcial = (*it).getRuta();

				cout << "menor despues: " << min << endl;

				cout << endl;
				cout << "Nodo ACTUAL: fil " << actual.getFil() << " col " << actual.getCol() << " costeF " << actual.getCosteF() << endl;
				cout << endl;
			}
		}

		cout << "hola ruta" << endl;
		
		while(!ruta_parcial.empty())
		{
			ruta.push_back(ruta_parcial.front());
			ruta_parcial.pop_front();
		}

		cout << "adios ruta" << endl;


		// eliminar nodo actual de abiertos
		abiertos.erase(find(abiertos.begin(), abiertos.end(), actual));

		// añadir actual a cerrados
		cerrados.push_back(actual);

		if(actual.getFil() == destino.fila && actual.getCol() == destino.columna)
		{
			cout << "Estamos en el objetivo" << endl;

			if(ruta.empty())
			{
				cout << "si, esta vacia" << endl;
			}
			else
			{
				cout << "no, no esta vacia" << endl;
			}

			plan = ruta;

			PintaPlan(plan);
			VisualizaPlan(origen, plan);

			return true;
		}

		// Comprobar los vecinos de actual
		vecinos.clear();

		Nodo aux_izq, aux_dcha, aux_frente;


		switch(actual.getOri())
		{
			case 0:
				// Nodo aux_izq, aux_dcha, aux_frente;

				vecinoIzq 		= mapaResultado[actual.getFil()][actual.getCol() - 1];
				vecinoDcha 		= mapaResultado[actual.getFil()][actual.getCol() + 1];
				vecinoFrente 	= mapaResultado[actual.getFil() - 1][actual.getCol()];
				
				cout << endl;
				cout << "Caso 0, vecinoIzq: " << vecinoIzq << " vecinoDcha: " << vecinoDcha << " vecfrente:" << vecinoFrente << endl;

				// Asignacion de costes
				cout << " *** Asignando costes *** " << endl;
				for(int i = 0; i < 3; i++)
				{
					if(i==0) // Si giro a la izq
					{
						cout << "-> si giro a la izq" << endl;
						aux_izq.setFil(actual.getFil());
						aux_izq.setCol(actual.getCol() - 1);
						aux_izq.setOri(actual.getOri());
						aux_izq.setRuta(actTURN_L);
						aux_izq.setRuta(actFORWARD);

						// si vecino izq es T, S o K, costeG=20 sino 99999 
						if(vecinoIzq == 'T' || vecinoIzq =='S' || vecinoIzq == 'K')
							aux_izq.setCosteG(20);
						else
							aux_izq.setCosteG(99999);

						// Coste h
						int costeH = distanciaManhattan(actual.getEstado(), destino);
						aux_izq.setCosteH(costeH);
						cout << "distanciaManhattan: " << costeH << endl;
						cout << "aux_izq costeH: " << aux_izq.getCosteH() << endl;
						cout << "aux_izq costeG: " << aux_izq.getCosteG() << endl;
						cout << "aux_izq costeF: " << aux_izq.getCosteF() << endl;

						/*Ya tengo los costes de vecino izq tengo q guardarlo y decidir, comparando si es mejor izq, dcha o frente */
						vecinos.push_back(aux_izq);
						cout << "acabo en vecino izq" << endl;
					}
					else if(i == 1) // si giro a la dcha
					{
						cout << "-> si giro a la dcha" << endl;
						aux_dcha.setFil(actual.getFil());
						aux_dcha.setCol(actual.getCol() + 1);
						aux_dcha.setOri(actual.getOri());
						aux_dcha.setRuta(actTURN_R);
						aux_dcha.setRuta(actFORWARD);

						// si vecino dcha es T, S o K, costeG=20 sino 99999 
						if(vecinoDcha == 'T' || vecinoDcha == 'S' || vecinoDcha == 'K')
							aux_dcha.setCosteG(20);
						else 
							aux_dcha.setCosteG(99999);

						// Coste h
						int costeH = distanciaManhattan(actual.getEstado(), destino);
						aux_dcha.setCosteH(costeH);
						cout << "distanciaManhattan: " << costeH << endl;
						cout << "aux_dcha costeH: " << aux_dcha.getCosteH() << endl;
						cout << "aux_dcha costeG: " << aux_dcha.getCosteG() << endl;
						cout << "aux_dcha costeF: " << aux_dcha.getCosteF() << endl;

						/*Ya tengo los costes de vecino izq tengo q guardarlo y decidir, comparando si es mejor izq, dcha o frente */
						vecinos.push_back(aux_dcha);
					}
					else
					{
						cout << "-> si voy de frente" << endl;
						aux_frente.setFil(actual.getFil() - 1);
						aux_frente.setCol(actual.getCol());
						aux_frente.setOri(actual.getOri());
						aux_frente.setRuta(actFORWARD);

						cout << "frente vecino: " << vecinoFrente << endl;

						// si vecino dcha no es T, S o K, costeG=99999 
						if(vecinoFrente == 'T' || vecinoFrente == 'S' || vecinoFrente == 'K')
						{
							cout << "entro en if" << endl;
							aux_frente.setCosteG(10);
						}

						else 
						{
							cout << "entro en else" << endl;
							aux_frente.setCosteG(99999);
						}

						// Coste h
						int costeH = distanciaManhattan(actual.getEstado(), destino);
						aux_frente.setCosteH(costeH);
						cout << "distanciaManhattan: " << costeH << endl;
						cout << "aux_frente costeH: " << aux_frente.getCosteH() << endl;
						cout << "aux_frente costeG: " << aux_frente.getCosteG() << endl;
						cout << "aux_frente costeF: " << aux_frente.getCosteF() << endl;
						cout << "aux_frente ruta: " << aux_frente.getRuta().front() << endl;

						/*Ya tengo los costes de vecino izq tengo q guardarlo y decidir, comparando si es mejor izq, dcha o frente */
						vecinos.push_back(aux_frente);
					}
				}

				break;
			case 1:
				// Nodo aux_izq, aux_dcha, aux_frente;

				vecinoIzq 		= mapaResultado[actual.getFil() - 1][actual.getCol()];
				vecinoDcha 		= mapaResultado[actual.getFil() + 1][actual.getCol()];
				vecinoFrente 	= mapaResultado[actual.getFil()][actual.getCol() + 1];
				
				cout << endl;
				cout << "Caso 1, vecinoIzq: " << vecinoIzq << " vecinoDcha: " << vecinoDcha << " vecfrente:" << vecinoFrente << endl;

				// Asignacion de costes
				cout << " *** Asignando costes *** " << endl;
				for(int i = 0; i < 3; i++)
				{
					if(i==0) // Si giro a la izq
					{
						cout << "-> si giro a la izq" << endl;
						aux_izq.setFil(actual.getFil() - 1);
						aux_izq.setCol(actual.getCol());
						aux_izq.setOri(actual.getOri());
						aux_izq.setRuta(actTURN_L);
						aux_izq.setRuta(actFORWARD);

						// si vecino izq es T, S o K, costeG=20 sino 99999 
						if(vecinoIzq == 'T' || vecinoIzq =='S' || vecinoIzq == 'K')
							aux_izq.setCosteG(20);
						else
							aux_izq.setCosteG(99999);

						// Coste h
						int costeH = distanciaManhattan(actual.getEstado(), destino);
						aux_izq.setCosteH(costeH);
						cout << "distanciaManhattan: " << costeH << endl;
						cout << "aux_izq costeH: " << aux_izq.getCosteH() << endl;
						cout << "aux_izq costeG: " << aux_izq.getCosteG() << endl;
						cout << "aux_izq costeF: " << aux_izq.getCosteF() << endl;

						/*Ya tengo los costes de vecino izq tengo q guardarlo y decidir, comparando si es mejor izq, dcha o frente */
						vecinos.push_back(aux_izq);
						cout << "acabo en vecino izq" << endl;
					}
					else if(i == 1) // si giro a la dcha
					{
						cout << "-> si giro a la dcha" << endl;
						aux_dcha.setFil(actual.getFil() + 1);
						aux_dcha.setCol(actual.getCol());
						aux_dcha.setOri(actual.getOri());
						aux_dcha.setRuta(actTURN_R);
						aux_dcha.setRuta(actFORWARD);

						// si vecino dcha es T, S o K, costeG=20 sino 99999 
						if(vecinoDcha == 'T' || vecinoDcha == 'S' || vecinoDcha == 'K')
							aux_dcha.setCosteG(20);
						else 
							aux_dcha.setCosteG(99999);

						// Coste h
						int costeH = distanciaManhattan(actual.getEstado(), destino);
						aux_dcha.setCosteH(costeH);
						cout << "distanciaManhattan: " << costeH << endl;
						cout << "aux_dcha costeH: " << aux_dcha.getCosteH() << endl;
						cout << "aux_dcha costeG: " << aux_dcha.getCosteG() << endl;
						cout << "aux_dcha costeF: " << aux_dcha.getCosteF() << endl;

						/*Ya tengo los costes de vecino izq tengo q guardarlo y decidir, comparando si es mejor izq, dcha o frente */
						vecinos.push_back(aux_dcha);
					}
					else
					{
						cout << "-> si voy de frente" << endl;
						aux_frente.setFil(actual.getFil());
						aux_frente.setCol(actual.getCol() + 1);
						aux_frente.setOri(actual.getOri());
						aux_frente.setRuta(actFORWARD);

						cout << "frente vecino: " << vecinoFrente << endl;

						// si vecino dcha no es T, S o K, costeG=99999 
						if(vecinoFrente == 'T' || vecinoFrente == 'S' || vecinoFrente == 'K')
						{
							cout << "entro en if" << endl;
							aux_frente.setCosteG(10);
						}

						else 
						{
							cout << "entro en else" << endl;
							aux_frente.setCosteG(99999);
						}

						// Coste h
						int costeH = distanciaManhattan(actual.getEstado(), destino);
						aux_frente.setCosteH(costeH);
						cout << "distanciaManhattan: " << costeH << endl;
						cout << "aux_frente costeH: " << aux_frente.getCosteH() << endl;
						cout << "aux_frente costeG: " << aux_frente.getCosteG() << endl;
						cout << "aux_frente costeF: " << aux_frente.getCosteF() << endl;

						/*Ya tengo los costes de vecino izq tengo q guardarlo y decidir, comparando si es mejor izq, dcha o frente */
						vecinos.push_back(aux_frente);
					}
				}


				break;
				
			// case 2:
			// 	// Nodo aux_izq, aux_dcha, aux_frente;

			// 	vecinoIzq 		= mapaResultado[actual.getFil()][actual.getCol() + 1];
			// 	vecinoDcha 		= mapaResultado[actual.getFil()][actual.getCol() - 1];
			// 	vecinoFrente 	= mapaResultado[actual.getFil() + 1][actual.getCol()];

			// 	cout << endl;
			// 	cout << "Caso 2, vecinoIzq: " << vecinoIzq << " vecinoDcha: " << vecinoDcha << " vecfrente:" << vecinoFrente << endl;

			// 	// Asignacion de costes
			// 	cout << " *** Asignando costes *** " << endl;
			// 	for(int i = 0; i < 3; i++)
			// 	{
			// 		if(i==0) // Si giro a la izq
			// 		{
			// 			cout << "-> si giro a la izq" << endl;
			// 			aux_izq.setFil(actual.getFil());
			// 			aux_izq.setCol(actual.getCol() + 1);
			// 			aux_izq.setOri(actual.getOri());
			// 			aux_izq.setRuta(actTURN_L);
			// 			aux_izq.setRuta(actFORWARD);

			// 			// si vecino izq es T, S o K, costeG=20 sino 99999 
			// 			if(vecinoIzq == 'T' || vecinoIzq =='S' || vecinoIzq == 'K')
			// 				aux_izq.setCosteG(20);
			// 			else
			// 				aux_izq.setCosteG(99999);

			// 			// Coste h
			// 			int costeH = distanciaManhattan(actual.getEstado(), destino);
			// 			aux_izq.setCosteH(costeH);
			// 			cout << "distanciaManhattan: " << costeH << endl;
			// 			cout << "aux_izq costeH: " << aux_izq.getCosteH() << endl;
			// 			cout << "aux_izq costeG: " << aux_izq.getCosteG() << endl;
			// 			cout << "aux_izq costeF: " << aux_izq.getCosteF() << endl;

			// 			/*Ya tengo los costes de vecino izq tengo q guardarlo y decidir, comparando si es mejor izq, dcha o frente */
			// 			vecinos.push_back(aux_izq);
			// 			cout << "acabo en vecino izq" << endl;
			// 		}
			// 		else if(i == 1) // si giro a la dcha
			// 		{
			// 			cout << "-> si giro a la dcha" << endl;
			// 			aux_dcha.setFil(actual.getFil());
			// 			aux_dcha.setCol(actual.getCol() - 1);
			// 			aux_dcha.setOri(actual.getOri());
			// 			aux_dcha.setRuta(actTURN_R);
			// 			aux_dcha.setRuta(actFORWARD);

			// 			// si vecino dcha es T, S o K, costeG=20 sino 99999 
			// 			if(vecinoDcha == 'T' || vecinoDcha == 'S' || vecinoDcha == 'K')
			// 				aux_dcha.setCosteG(20);
			// 			else 
			// 				aux_dcha.setCosteG(99999);

			// 			// Coste h
			// 			int costeH = distanciaManhattan(actual.getEstado(), destino);
			// 			aux_dcha.setCosteH(costeH);
			// 			cout << "distanciaManhattan: " << costeH << endl;
			// 			cout << "aux_dcha costeH: " << aux_dcha.getCosteH() << endl;
			// 			cout << "aux_dcha costeG: " << aux_dcha.getCosteG() << endl;
			// 			cout << "aux_dcha costeF: " << aux_dcha.getCosteF() << endl;

			// 			/*Ya tengo los costes de vecino izq tengo q guardarlo y decidir, comparando si es mejor izq, dcha o frente */
			// 			vecinos.push_back(aux_dcha);
			// 		}
			// 		else
			// 		{
			// 			cout << "-> si voy de frente" << endl;
			// 			aux_frente.setFil(actual.getFil() + 1);
			// 			aux_frente.setCol(actual.getCol());
			// 			aux_frente.setOri(actual.getOri());
			// 			aux_frente.setRuta(actFORWARD);

			// 			cout << "frente vecino: " << vecinoFrente << endl;

			// 			// si vecino dcha no es T, S o K, costeG=99999 
			// 			if(vecinoFrente == 'T' || vecinoFrente == 'S' || vecinoFrente == 'K')
			// 			{
			// 				cout << "entro en if" << endl;
			// 				aux_frente.setCosteG(10);
			// 			}

			// 			else 
			// 			{
			// 				cout << "entro en else" << endl;
			// 				aux_frente.setCosteG(99999);
			// 			}

			// 			// Coste h
			// 			int costeH = distanciaManhattan(actual.getEstado(), destino);
			// 			aux_frente.setCosteH(costeH);
			// 			cout << "distanciaManhattan: " << costeH << endl;
			// 			cout << "aux_frente costeH: " << aux_frente.getCosteH() << endl;
			// 			cout << "aux_frente costeG: " << aux_frente.getCosteG() << endl;
			// 			cout << "aux_frente costeF: " << aux_frente.getCosteF() << endl;

			// 			/*Ya tengo los costes de vecino izq tengo q guardarlo y decidir, comparando si es mejor izq, dcha o frente */
			// 			vecinos.push_back(aux_frente);
			// 		}
			// 	}

			// 	break;
			
			// case 3:
			// 	// Nodo aux_izq, aux_dcha, aux_frente;

			// 	vecinoIzq 		= mapaResultado[actual.getFil() + 1][actual.getCol()];
			// 	vecinoDcha 		= mapaResultado[actual.getFil() - 1][actual.getCol()];
			// 	vecinoFrente 	= mapaResultado[actual.getFil()][actual.getCol() - 1];

			// 	cout << endl;
			// 	cout << "Caso 3, vecinoIzq: " << vecinoIzq << " vecinoDcha: " << vecinoDcha << " vecfrente:" << vecinoFrente << endl;

			// 	// Asignacion de costes
			// 	cout << " *** Asignando costes *** " << endl;
			// 	for(int i = 0; i < 3; i++)
			// 	{
			// 		if(i==0) // Si giro a la izq
			// 		{
			// 			cout << "-> si giro a la izq" << endl;
			// 			aux_izq.setFil(actual.getFil() + 1);
			// 			aux_izq.setCol(actual.getCol());
			// 			aux_izq.setOri(actual.getOri());
			// 			aux_izq.setRuta(actTURN_L);
			// 			aux_izq.setRuta(actFORWARD);

			// 			// si vecino izq es T, S o K, costeG=20 sino 99999 
			// 			if(vecinoIzq == 'T' || vecinoIzq =='S' || vecinoIzq == 'K')
			// 				aux_izq.setCosteG(20);
			// 			else
			// 				aux_izq.setCosteG(99999);

			// 			// Coste h
			// 			int costeH = distanciaManhattan(actual.getEstado(), destino);
			// 			aux_izq.setCosteH(costeH);
			// 			cout << "distanciaManhattan: " << costeH << endl;
			// 			cout << "aux_izq costeH: " << aux_izq.getCosteH() << endl;
			// 			cout << "aux_izq costeG: " << aux_izq.getCosteG() << endl;
			// 			cout << "aux_izq costeF: " << aux_izq.getCosteF() << endl;

			// 			/*Ya tengo los costes de vecino izq tengo q guardarlo y decidir, comparando si es mejor izq, dcha o frente */
			// 			vecinos.push_back(aux_izq);
			// 			cout << "acabo en vecino izq" << endl;
			// 		}
			// 		else if(i == 1) // si giro a la dcha
			// 		{
			// 			cout << "-> si giro a la dcha" << endl;
			// 			aux_dcha.setFil(actual.getFil() - 1);
			// 			aux_dcha.setCol(actual.getCol());
			// 			aux_dcha.setOri(actual.getOri());
			// 			aux_dcha.setRuta(actTURN_R);
			// 			aux_dcha.setRuta(actFORWARD);

			// 			// si vecino dcha es T, S o K, costeG=20 sino 99999 
			// 			if(vecinoDcha == 'T' || vecinoDcha == 'S' || vecinoDcha == 'K')
			// 				aux_dcha.setCosteG(20);
			// 			else 
			// 				aux_dcha.setCosteG(99999);

			// 			// Coste h
			// 			int costeH = distanciaManhattan(actual.getEstado(), destino);
			// 			aux_dcha.setCosteH(costeH);
			// 			cout << "distanciaManhattan: " << costeH << endl;
			// 			cout << "aux_dcha costeH: " << aux_dcha.getCosteH() << endl;
			// 			cout << "aux_dcha costeG: " << aux_dcha.getCosteG() << endl;
			// 			cout << "aux_dcha costeF: " << aux_dcha.getCosteF() << endl;

			// 			/*Ya tengo los costes de vecino izq tengo q guardarlo y decidir, comparando si es mejor izq, dcha o frente */
			// 			vecinos.push_back(aux_dcha);
			// 		}
			// 		else
			// 		{
			// 			cout << "-> si voy de frente" << endl;
			// 			aux_frente.setFil(actual.getFil());
			// 			aux_frente.setCol(actual.getCol() - 1);
			// 			aux_frente.setOri(actual.getOri());
			// 			aux_frente.setRuta(actFORWARD);

			// 			cout << "frente vecino: " << vecinoFrente << endl;

			// 			// si vecino dcha no es T, S o K, costeG=99999 
			// 			if(vecinoFrente == 'T' || vecinoFrente == 'S' || vecinoFrente == 'K')
			// 			{
			// 				cout << "entro en if" << endl;
			// 				aux_frente.setCosteG(10);
			// 			}

			// 			else 
			// 			{
			// 				cout << "entro en else" << endl;
			// 				aux_frente.setCosteG(99999);
			// 			}

			// 			// Coste h
			// 			int costeH = distanciaManhattan(actual.getEstado(), destino);
			// 			aux_frente.setCosteH(costeH);
			// 			cout << "distanciaManhattan: " << costeH << endl;
			// 			cout << "aux_frente costeH: " << aux_frente.getCosteH() << endl;
			// 			cout << "aux_frente costeG: " << aux_frente.getCosteG() << endl;
			// 			cout << "aux_frente costeF: " << aux_frente.getCosteF() << endl;

			// 			/*Ya tengo los costes de vecino izq tengo q guardarlo y decidir, comparando si es mejor izq, dcha o frente */
			// 			vecinos.push_back(aux_frente);
			// 		}
			// 	}

			// 	break;
		}

		// Ahora tengo que recorrer los vecinos y ver cual es el que me interesa 
		// creo q hay que meterlos en abiertos y elegir

		cout << endl;
		for(auto it = vecinos.begin(); it != vecinos.end(); ++it)
		{
			cout << "vecinos: " << "(" << (*it).getFil() << "," << (*it).getCol() << "), costeF: " << (*it).getCosteF() << endl;

		}

		Nodo vecino_mejor = vecinos.front();

		cout << endl;
		for(auto it = vecinos.begin(); it != vecinos.end(); ++it)
		{
			cout << "it fil: " << (*it).getFil() << " col:" << (*it).getCol() << ", costeF: " << (*it).getCosteF() << endl;
			for(auto it_sig = vecinos.begin(); it_sig != vecinos.end(); ++it_sig)
			{
				cout << "it_sig fil: " << (*it_sig).getFil() << " col:" << (*it_sig).getCol() << ", costeF: " << (*it_sig).getCosteF() << endl;
				if((*it).getCosteF() <= (*it_sig).getCosteF() && (*it).getCosteF() <= vecino_mejor.getCosteF())
				{
					cout << "soy menorrrrrrr" << endl;
					
					// vecino_mejor = (*it);
					vecino_mejor.setEstado((*it).getEstado()) ;

					list<Action> ruta_vecino_mejor = vecino_mejor.getRuta();
					while(!ruta_vecino_mejor.empty())
					{
						vecino_mejor.setRuta(ruta_vecino_mejor.front());
						ruta_vecino_mejor.pop_front();
					}
					
					cout << "vecino mejor: " << vecino_mejor.getFil() << "," << vecino_mejor.getCol() << endl;
					cout << endl;
				}
			}
		}

		cout << "\n *** vecino mejor: " << vecino_mejor.getFil() << "," << vecino_mejor.getCol() << endl;

		// int min_vecinos = vecino_mejor.getCosteF();
		// cout << "coste f del min " << min_vecinos << endl;

		

		// for(auto it = vecinos.begin(); it != vecinos.end(); ++it)
		// {
		// 	auto existe = find(abiertos.begin(), abiertos.end(), (*it));
		// 	if((*it).getCosteG() != 99999 && find(cerrados.begin(), cerrados.end(), (*it)) == cerrados.end() && (*it).getCosteF() == min_vecinos)
		// 	{	
		// 		cout << "no estoy en cerrados primo " << endl;
		// 		cout << "soy nodo, fil: " << (*it).getFil() << " col: " << (*it).getCol() << " costeF: " << (*it).getCosteF() << endl;
		// 		if(existe == abiertos.end())
		// 		{	
		// 			cout << "coste f del min " << min_vecinos << endl;
		// 			cout << "no estoy en abiertos primo " << endl;
		// 			cout << "ruta: " << (*it).getRuta().front() << " ******" << endl;
		// 			abiertos.push_back((*it));
		// 		}
		// 	}
		// }
		// si esta en abiertos mirar si el camino es mejor

		cout << "holaaaaaa" << endl;

		auto existe = find(abiertos.begin(), abiertos.end(), vecino_mejor);

		cout << "holaaaaaa 2" << endl;

		cout << "vecino_mejor, costeg: " << vecino_mejor.getCosteG() << endl;

		if(vecino_mejor.getCosteG() != 99999 && find(cerrados.begin(), cerrados.end(), vecino_mejor) == cerrados.end())
		{	
			cout << "no estoy en cerrados primo " << endl;
			cout << "soy nodo, fil: " << vecino_mejor.getFil() << " col: " << vecino_mejor.getCol() << " costeF: " << vecino_mejor.getCosteF() << endl;
			if(existe == abiertos.end())
			{	
				cout << "no estoy en abiertos primo " << endl;
				cout << "ruta: " << vecino_mejor.getRuta().front() << " ******" << endl;
				abiertos.push_back(vecino_mejor);
			}
		}
		
		
		cout << " me piro " << endl;
	}
	return false;
}

Action ComportamientoJugador::think(Sensores sensores)
{
	// capturar los valores de fil y col
	// devuelve -1 si no hay información
	if (sensores.mensajeF != -1)
  	{
		fil = sensores.mensajeF;
		col = sensores.mensajeC;
	}

	// Actualizar el efecto de la ultima accion
	switch (ultimaAccion)
	{
		case actTURN_R: brujula = (brujula+1)%4; break;
		case actTURN_L: brujula = (brujula+3)%4; break;
		case actFORWARD:
			switch (brujula)
			{
				case 0: fil--; break;
				case 1: col++; break;
				case 2: fil++; break;
				case 3: col--; break;
			}
			cout << "fil: " << fil << "  col: " << col << " Ori: " << brujula << endl;
	}
	cout << "Destino fil: " << sensores.destinoF << "  col: " << sensores.destinoC << endl;

	if (sensores.reset)
	{
		hayPlan = false;
	}

	// Determinar si ha cambiado el destino desde la ultima planificacion
	if (hayPlan and (sensores.destinoF != ultPosF or sensores.destinoC != ultPosC))
	{
		cout << "El destino ha cambiado \n";
		hayPlan = false;
	}

	// Determinar si tengo que construir un plan
	if (!hayPlan)
	{
		estado origen, destino;
		origen.fila = fil;
		origen.columna = col;
		origen.orientacion = brujula;

		destino.fila = sensores.destinoF;
		destino.columna = sensores.destinoC;

    	hayPlan = pathFinding(origen,destino,plan);

		ultPosF = sensores.destinoF;
		ultPosC = sensores.destinoC;
	}


	// Ejecutar el plan
	Action sigAccion;
	if (hayPlan and plan.size()>0)
	{
		sigAccion = plan.front();
		plan.erase(plan.begin());
	}
	else
	{
		sigAccion = actIDLE;
	}

	ultimaAccion = sigAccion;
	return sigAccion;
}


void AnularMatriz(vector<vector<unsigned char> > &m)
{
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}

void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan)
{
  AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end())
	{
		if (*it == actFORWARD)
		{
			switch (cst.orientacion)
			{
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R)
		{
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else
		{
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}

int ComportamientoJugador::interact(Action accion, int valor)
{
  return false;
}

bool ComportamientoJugador::Nodo::operator=(Nodo &n) const
{
	return stNodo.fila == n.getFil() && stNodo.columna == n.getCol() && stNodo.orientacion == n.getOri() && costeG == n.getCosteG() && costeH == n.getCosteH() && costeF == n.getCosteF();
}

bool ComportamientoJugador::Nodo::operator==(Nodo &n) const
{
	return stNodo.fila == n.getFil() && stNodo.columna == n.getCol() && stNodo.orientacion == n.getOri();
}

bool ComportamientoJugador::Nodo::operator!=(Nodo &n) const
{
	return stNodo.fila != n.getFil() && stNodo.columna != n.getCol() && stNodo.orientacion != n.getOri();
}

bool ComportamientoJugador::Nodo::operator=(const Nodo &n) const
{
	return stNodo.fila == n.getFil() && stNodo.columna == n.getCol() && stNodo.orientacion == n.getOri() && costeG == n.getCosteG() && costeH == n.getCosteH() && costeF == n.getCosteF();
}

bool ComportamientoJugador::Nodo::operator==(const Nodo &n) const
{
	return stNodo.fila == n.getFil() && stNodo.columna == n.getCol() && stNodo.orientacion == n.getOri();
}

bool ComportamientoJugador::Nodo::operator!=(const Nodo &n) const
{
	return stNodo.fila != n.getFil() && stNodo.columna != n.getCol() && stNodo.orientacion != n.getOri();
}

int ComportamientoJugador::distanciaManhattan(const estado &e1, const estado &e2)
{
	return abs(e1.fila - e2.fila) + abs(e1.columna - e2.columna);
}

// ostream& ComportamientoJugador::Nodo::operator<<(ostream &os, Nodo &n)
// {
//   os << "(" << n.getFil() << ", " << n.getCol() << ", " << n.getOri() << ", " << n.get() << ")";
//   return os;
// };
