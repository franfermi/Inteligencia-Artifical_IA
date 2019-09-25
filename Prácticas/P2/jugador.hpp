#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>
#include <iostream>

struct estado {
  int fila;
  int columna;
  int orientacion;
};

class ComportamientoJugador : public Comportamiento 
{
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) 
    {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
      ultimaAccion = actIDLE;
      hayPlan = false;
      estoy_bien_situado = false;
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) 
    {
      // Inicializar Variables de Estado
      fil = col = 99;
      brujula = 0; // 0: Norte, 1:Este, 2:Sur, 3:Oeste
      destino.fila = -1;
      destino.columna = -1;
      destino.orientacion = -1;
      ultimaAccion = actIDLE;
      hayPlan = false;
      estoy_bien_situado = false;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void VisualizaPlan(const estado &st, const list<Action> &plan);

    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

  private:
    // Declarar Variables de Estado
    int fil, col, brujula;
    estado destino;
    list<Action> plan;

    // Nuevas Variables de Estado
    Action ultimaAccion;
    bool hayPlan, estoy_bien_situado;
    int ultPosF, ultPosC;


    bool pathFinding(const estado &origen, const estado &destino, list<Action> &plan);
    void PintaPlan(list<Action> plan);

    class Nodo
    {
       private:
        estado stNodo;
        int costeG, costeH, costeF;
        // Nodo *padre;
        list<Action> ruta;

      public:
        Nodo() {}
        // Nodo(estado st, Nodo *papa):stNodo(st), padre(papa){}

        Nodo(estado st, int g, int h, int f):stNodo(st), costeG(g), costeH(h), costeF(f){}

        void setFil(int fil)
        {
          stNodo.fila = fil;
        }

        void setCol(int col)
        {
          stNodo.columna = col;
        }

        void setOri(int ori)
        {
          stNodo.orientacion = ori;
        }

        void setEstado(estado st)
        {
          stNodo = st;
        }

        void setCosteG(int costeG)
        {
          this->costeG = costeG;
        }

        void setCosteH(int costeH)
        {
          this->costeH = costeH;
        }

        void setCosteF(int costeF)
        {
          this->costeF = costeF;
        }

        void setRuta(Action a)
        {
          ruta.push_back(a);
        }

        estado getEstado() const
        {
          return stNodo;
        }

        int getFil() const
        {
          return stNodo.fila;
        }

        int getCol() const
        {
          return stNodo.columna;
        }

        int getOri() const
        {
          return stNodo.orientacion;
        }

        int getCosteG() const
        {
          return costeG;
        }

        int getCosteH() const
        {
          return costeH;
        }

        int getCosteF() const
        {
          return costeG + costeH;
          // return costeF;
        }

        list<Action> getRuta() const
        {
          return ruta;
        }

        bool operator=(Nodo &n) const;

        bool operator==(Nodo &n) const;

        bool operator!=(Nodo &n) const;

        bool operator=(const Nodo &n) const;

        bool operator==(const Nodo &n) const;

        bool operator!=(const Nodo &n) const;
        
    };

    class Distancia
    {
      public:
        bool operator()(Nodo &n1, Nodo &n2)
        {
          return n1.getCosteF() > n2.getCosteF() || (n1.getCosteF() == n2.getCosteF() && n1.getCosteH() > n2.getCosteH());
        }
    };

  public:
    int distanciaManhattan(const estado &e1, const estado &e2);
    
};

#endif
