#ifndef DRONE_H
#define DRONE_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include "Constantes.h"
const int V = 10; // cantidad maxima de malezas

using namespace std;

// Definiremos las posicion de salida del drone
  struct Drone{
      char formaDrone = 'D';
      int posX;
      int posY;
  Drone(){ posX = 1; posY = 1; }
  };

// Definiremos las posicion del centro de conjunto de maleza
class Maleza {
private:
    int posX;
    int posY;


public:
    Maleza() : posX(0), posY(0) {}  // Constructor por defecto con posiciones en (0, 0)
    Maleza(int x, int y) : posX(x), posY(y) {}  // Constructor que inicializa con posiciones específicas

    int getPosX() { return posX; }  // Método para obtener la posición X
    int getPosY() { return posY; }  // Método para obtener la posición Y

    void setPosX(int x) { posX = x; }  // Método para establecer la posición X
    void setPosY(int y) { posY = y; }  // Método para establecer la posición Y
};

class Barrera {
private:
    int posX;
    int posY;
    int largo;
    string tipo; //Horizontal, vertical

public:
    Barrera() : posX(0), posY(0), largo(0), tipo("") {}  // Constructor por defecto con posiciones en (0, 0)
    Barrera(int x, int y, int large, string type) : posX(x), posY(y), largo(large), tipo(type) {}  // Constructor que inicializa con posiciones específicas

    int getPosX() { return posX; }  // Método para obtener la posición X
    int getPosY() { return posY; }  // Método para obtener la posición Y
    int getLargo() { return largo;} // Metodo para obtener el largo de la barrera
    string getTipo() { return tipo; }  // Método para obtener el tipo de barrera

    void setPosX(int x) { posX = x; }  // Método para establecer la posición X
    void setPosY(int y) { posY = y; }  // Método para establecer la posición Y
    void setTipo(string type) { tipo = type; } // Metodo para establecer el tipo de barrera
    void setLargo(int large) { largo = large;} // Metodo para establecer el largo de la barrera
};


class Point{
    private:
        int posX;
        int posY;
    public:
        Point() : posX(0), posY(0) {};
        Point(int x, int y) : posX(x), posY(y) {};

        void setX(int x){posX = x;}
        void setY(int y){posY = y;}

        int getX(){return posX;}
        int getY(){return posY;}
};

// almacena V-1 elementos
// cola circular
class Cola{
    private:
        int Q[V];
        int pi,pf;
    public:
        Cola(void){pi=0;pf=V-1;};
        //~Cola(void);
        int tope(){return Q[pi];};
        bool colavacia(){return pi==(pf+1)%V;};
        bool colallena(){return pi==(pf+2)%V;};
        void encolar(int a);
        void desencolar();
        int ultimo();
        string imprimir(string s);
};
//-------- Metodos de Cola --------------------
void Cola::encolar(int a)
{   if (!colallena()){
        pf=(pf+1)%V;
        Q[pf]=a; 
    }
    //else cout<<"Cola llena"<<endl;  
}
void Cola::desencolar(void)
{   if (!colavacia())pi=(pi+1)%V;

}
int Cola::ultimo(void)
{   return Q[pf];
}
string Cola::imprimir(string s)
{    if(!colavacia()){  
        //by Martin     
        for(int i=pi;i!=pf+1;i=(i+1)%V)
            if(i!=pf)
            {cout<<Q[i]<<"->";}
            else if (Q[i]==0){ cout << Q[i];}
        else { cout << Q[i]<<"->0"; /* aca deberia ir pi en lugar de 0*/}
    }             
    return " <<< Q";
};

// Le pasamos una coordenada x e y, y va a devolver una Imagen con la posicion del drone
    void modificarPos(int x, int y, char Imagen[MAX_PIXLS][MAX_PIXLS]) {
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    Imagen[x + i][y + j] = Drone().formaDrone;
                }
            }
        };

    
//Encontrar maleza en x, y
bool malezaEsta(char mat[MAX_PIXLS][MAX_PIXLS], int x, int y) {
    // Verifica que x y y están en los límites válidos de la matriz
    if (x > 0 && x < MAX_PIXLS - 1 && y > 0 && y < MAX_PIXLS - 1) {
        // Verifica que todos los elementos alrededor de la posición [x][y] son '1'
        if (mat[x-1][y-1] == '1' && mat[x][y-1] == '1' && mat[x+1][y-1] == '1' &&
            mat[x-1][y] == '1' && mat[x][y] == '1' && mat[x+1][y] == '1' &&
            mat[x-1][y+1] == '1' && mat[x][y+1] == '1' && mat[x+1][y+1] == '1') {
            return true;
        }
    }
    return false;
}


// Filtro de busqueda de malezas importantes,

    Maleza* filtroMaleza(char mat[MAX_PIXLS][MAX_PIXLS],int &cont){
        Maleza* malezas = new Maleza[V];
        
        for(int f=0; f<MAX_PIXLS; f++){
            for(int c=0; c<MAX_PIXLS; c++){
                if(malezaEsta(mat, f, c) && cont < V){
                    malezas[cont] = Maleza(c, f);
                    //malezas[cont].setPosX(f);
                    //malezas[cont].getPosX();
                    cont++;
                }
            }
        }
        
        return malezas;
    }

    //Retorna lista de barreras donde la posicion de la barrera es la posicion inicial, con su largo correspondiente y tipo horizontal
    Barrera* filtroBarreraHorizontal(char mat[MAX_PIXLS][MAX_PIXLS],int &cont){
        bool esta = false;
        int inicio=-1;
        Barrera* barreras = new Barrera[100];
        for(int f=0; f<MAX_PIXLS; f++){
            for(int c=0; c<MAX_PIXLS; c++){
                //inicio de barrera horizontal
                if(c<MAX_PIXLS){
                    if(mat[f][c]=='x' && mat[f][c+1]=='x' && esta==false && cont<MAX_PIXLS && c<MAX_PIXLS){
                        esta = true;
                        inicio = c;
                        barreras[cont] = Barrera(c, f, 0, "Horizontal");
                        cont++;
                    }
                }
                //fin de barrera horizontal antes de MAX_PIXLS
                if(esta == true && mat[f][c-1]=='x' && mat[f][c]!='x' && cont<MAX_PIXLS && c<MAX_PIXLS-1){    
                    barreras[cont-1].setLargo(abs(c-inicio));
                    esta = false;
                    inicio = -1;
                }
                //fin de barreera horizontal en MAX_PIXLS
                if(esta == true && c==MAX_PIXLS-1 && cont<MAX_PIXLS && c==MAX_PIXLS-1){    
                    barreras[cont-1].setLargo(abs(c-inicio));
                    esta = false;
                    inicio = -1;
                }
            }
        }
    return barreras;
    }

    Barrera* filtroBarreraVertical(char mat[MAX_PIXLS][MAX_PIXLS],int &cont){
        bool esta = false;
        int inicio=-1;
        Barrera* barreras = new Barrera[100];
        for(int c=0; c<MAX_PIXLS; c++){
            for(int f=0; f<MAX_PIXLS; f++){
                //inicio de barrera horizontal
                if(f<MAX_PIXLS){ //Ya estoy
                    if(mat[f][c]=='x' && mat[f+1][c]=='x' && esta==false && cont<MAX_PIXLS){
                        esta = true;
                        inicio = f;
                        barreras[cont] = Barrera(c, f, 0, "Vertical");
                        cont++;
                    }
                }
                //fin de barrera vertical antes de MAX_PIXLS
                if(esta == true && mat[f-1][c]=='x' && mat[f][c]!='x' && cont<MAX_PIXLS && f<MAX_PIXLS-1){    
                    barreras[cont-1].setLargo(abs(f-inicio));
                    esta = false;
                    inicio = -1;
                }
                //fin de barrera vertical en MAX_PIXLS
                if(esta == true && f==MAX_PIXLS-1 && cont<MAX_PIXLS && f==MAX_PIXLS-1){    
                    barreras[cont-1].setLargo(abs(f-inicio));
                    esta = false;
                    inicio = -1;
                }
            }
        }
    return barreras;
    }

    void posMalezas(int X[10], int Y[10], int cant_malezas){

        cout<<"Cantidad de malezas: "<<cant_malezas<<endl;
        
        cout<<"POSICIONES DE LAS MALEZAS:"<<endl;
        for (int i = 0; i < cant_malezas; ++i){
            cout<<"Maleza "<<i<<": "<<X[i]<<endl;
            cout<<"Maleza "<<i<<": "<<Y[i]<<endl;
            cout<<endl;
        }
    }


    //Distancia entree malezas
    int dist_malezas(Maleza mal1, Maleza mal2){
        //return sqrt(pow(mal1.getPosX()-mal2.getPosX(),2)+pow(mal1.getPosY()-mal2.getPosY(),2));
        return modulo( mal1.getPosX()-mal2.getPosX(),mal1.getPosY()-mal2.getPosY() );
    }
     
//Algoritmos para identificar barreras entre nodos

// Dados tres puntos colineales p, q, r, la función verifica si
// el punto q está sobre el segmento de línea 'pr'
bool onSegment(Point p, Point q, Point r)
{
    if (q.getX() <= max(p.getX(), r.getX()) && q.getX() >= min(p.getX(), r.getX()) &&
        q.getY() <= max(p.getY(), r.getY()) && q.getY() >= min(p.getY(), r.getY()))
       return true;

    return false;
}

// Para encontrar la orientación del trío ordenado (p, q, r).
// La función devuelve los siguientes valores:
// 0 --> p, q y r son colineales
// 1 --> En sentido horario
// 2 --> En sentido antihorario
int orientation(Point p, Point q, Point r)
{
    int val = (q.getY() - p.getY()) * (r.getX() - q.getX()) -
              (q.getX() - p.getX()) * (r.getY() - q.getY());

    if (val == 0) return 0;  // colineales

    return (val > 0)? 1: 2; // en sentido horario o antihorario
}

// La función principal que devuelve true si el segmento de línea 'p1q1'
// y 'p2q2' se intersectan.
bool doIntersect(Point p1, Point q1, Point p2, Point q2)
{
    // Encontrar las cuatro orientaciones necesarias para casos generales y especiales
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // Caso general
    if (o1 != o2 && o3 != o4)
        return true;

    // Casos especiales
    // p1, q1 y p2 son colineales y p2 está sobre el segmento p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 y p2 son colineales y q2 está sobre el segmento p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 y p1 son colineales y p1 está sobre el segmento p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

     // p2, q2 y q1 son colineales y q1 está sobre el segmento p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; // No cae en ninguno de los casos anteriores
}

//Metodo conversor

bool malezaBarrera(Maleza mal1, Maleza mal2, Barrera barrera){
    Point m1 = Point(mal1.getPosX(), mal1.getPosY());
    Point m2 = Point(mal2.getPosX(), mal2.getPosY());
    Point b1 = Point(barrera.getPosX(), barrera.getPosY());
    Point b2;
    if(barrera.getTipo() == "Horizontal"){
        b2.setX(barrera.getPosX()+barrera.getLargo());
        b2.setY(barrera.getPosY());
    }else{
        b2.setX(barrera.getPosX());
        b2.setY(barrera.getPosY()+barrera.getLargo());
    }
    return doIntersect(m1, m2, b1, b2);
}//;

//--------------------Ciclos Hamiltonianos--------------------------//

//Función que imprime un ciclo Hamiltoniano dado
void printHamiltoniano(int path[], int vertices, int count, int pesos[][V], int &minway, Cola *cola) {
    //Imprime el título del n ciclo Hamiltoniano
    cout << count <<". Ciclo Hamiltoniano: ";
    
    //Itera sobre todos los vértices en el ciclo
    for (int i = 0; i < vertices; i++) {
        cout << path[i] << " "; // Imprime el número del vértice seguido de un espacio   
    }

    //Imprime el primer vértice nuevamente para cerrar el ciclo
    cout << path[0] << "; peso del camino: ";

    int peso_actual = 0;
    string camino = "";
    for(int i = 0; i < vertices; i++){
        if(i < vertices - 1){
            cout << pesos[path[i]][path[i+1]] << " + ";
            peso_actual += pesos[path[i]][path[i+1]];
            camino += to_string(path[i]);
        }
        else{
            cout << pesos[path[i]][path[0]]<<" = ";
            peso_actual += pesos[path[i]][path[0]];
            cout << peso_actual << endl;
            camino += to_string(path[i]); 
        }
    }
    
    // Compara el peso actual con el mínimo encontrado hasta ahora (min_way)
    if (peso_actual < minway) {
        if (!cola->colallena()){
            for(int i = 0; i < camino.length(); i++)
                { cola->desencolar(); }
        }
        if (cola->colavacia()){
            for(int i = 0; i < camino.length(); i++)
                { cola->encolar(camino.at(i)-48); } /* Encola los datos acumulados en el string camino (string de enteros) y se le resta el valor ASCII del numero 0 para que cargue el entero correcto y no el valor del char*/ 
        }
        minway = peso_actual; // Actualiza el mínimo si el peso actual es menor 
    }
}

//Función que verifica si es seguro agregar un vértice al camino Hamiltoniano
bool esSeguro(int v, int pos, int path[], int graph[V][V], bool visitado[]) {
    // Verifica si no hay una conexión entre el vértice anterior y el actual
    if (graph[path[pos - 1]][v] == 0) {
        return false; // No es seguro continuar por este camino
    }

    // Verifica si el vértice ya fue visitado antes
    if (visitado[v]) {
        return false; // No es seguro visitar nuevamente este vértice
    }

    return true; // Es seguro visitar este vértice
}

//Función recursiva que busca todos los ciclos Hamiltonianos en el grafo utilizando backtracking
void buscarHamiltonUtil(int graph[V][V], int path[], int pos, bool visitado[], int &count, int vertices, int pesos[][V], int &minway, Cola *cola) {
    
    //Si hemos completado el ciclo Hamiltoniano
    if (pos == vertices) {
        // Verifica si hay una conexión del último vértice al primer vértice
        if (graph[path[pos - 1]][path[0]] == 1) {
            printHamiltoniano(path, vertices, count, pesos, minway, cola); // Imprime el ciclo Hamiltoniano encontrado
            count++; // Incrementa el contador de ciclos encontrados
        }
    }

    //Itera sobre todos los vértices del grafo
    for (int v = 1; v < vertices; v++) {
        // Sii es seguro agregar este vértice al camino
        if (esSeguro(v, pos, path, graph, visitado)) {
            path[pos] = v; // Agrega el vértice al camino actual
            visitado[v] = true; // Marca el vértice como visitado

            // Llama recursivamente para encontrar más ciclos Hamiltonianos
            buscarHamiltonUtil(graph, path, pos + 1, visitado, count, vertices, pesos, minway, cola);

            path[pos] = -1; // Retrocede (backtrack) eliminando el vértice del camino
            visitado[v] = false; // Marca el vértice como no visitado
        }
    }
}

//Función principal que inicializa las estructuras de datos necesarias y llama a "buscarHamiltonUtil" para encontrar y mostrar todos los ciclos Hamiltonianos
void buscarHamilton(int graph[V][V], int vertices, int pesos[][V], Cola *cola) {
    int path[vertices]; // Arreglo para almacenar el camino actual
    //"visitado" se asegura que cada vértice se visite solamente una vez
    //en cada camino Hamiltoniano que se está construyendo
    bool visitado[vertices]; // Arreglo para marcar los vértices visitados
    int count = 0; // Contador para contar los ciclos Hamiltonianos encontrados
    int min_way = INF; // Variable para almacenar el ciclo de menor peso

    // Inicializa los arreglos path y visitado
    for (int i = 0; i < vertices; i++) {
        path[i] = -1; // Inicialmente ningún vértice está en el camino
        visitado[i] = false; // Inicialmente ningún vértice está visitado
    }

    path[0] = 0; // Comienza desde el vértice 0
    visitado[0] = true; // Marca el vértice 0 como visitado
    
    // Llama a la función que utilizamos para encontrar ciclos Hamiltonianos
    buscarHamiltonUtil(graph, path, 1, visitado, count, vertices, pesos, min_way, cola);

    // Si no se encontraron ciclos Hamiltonianos, imprime el sig. mensaje
    if (count == 0)
        cout <<"No se encontro ningun ciclo Hamiltoniano"<< endl;
    else
        cout<<"El camino con menor peso es: "<<min_way<<endl;
};
    
#endif