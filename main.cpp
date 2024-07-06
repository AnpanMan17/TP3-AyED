#include <fstream>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <iomanip>
#include "Drone.h"
#include "Constantes.h"

using namespace std;

int main() {
    
     //Conexion al txt de entrada
    ifstream archivo("Archivotext3.txt");
    if (!archivo) 
    {
        cout << "No se pudo abrir el archivo." <<endl;
        return 1;
    }

    // Leeremos el archivo línea por línea y contaremos las filas que nos hagan falta modificar en nuestra matriz
    int contadorLineas = 0;
    string linea;
    while (getline(archivo, linea)) { contadorLineas++; }

    // Con el archivo leido asignaremos los datos obtenidos
    int numeroFilas = contadorLineas;
    int numeroColumnas = linea.size();

    // Creamos la matriz de 100x100 pixeles que contendra la imagen con la informacion del terreno 
    char ImgCampo[MAX_PIXLS][MAX_PIXLS];

    cout << "Filas leidas: " <<  numeroFilas << " Columnas leidas: " << numeroColumnas << endl;

    // Creamos una imagen de pixeles vacios (se asignara "." para indicar que no hay informacion del terrreno)
    for (int f = 0; f < MAX_PIXLS; f++)
    {
        for (int c = 0; c < MAX_PIXLS; c++)
        {
            ImgCampo[f][c] = '.';
        }
    }

    // Reiniciar el archivo para volver a leerlo
    archivo.clear();   // Limpiar el estado de error
    archivo.seekg(0);  // Volver al principio del archivo


    // Cargamos la informacion del terreno a nuestra imagen
    for (int f = 0; f < numeroFilas; f++) {
        for (int c = 0; c < numeroColumnas; c++) {
            char caracter;
            archivo.get(caracter); // Leer un carácter del archivo
            
            if (caracter == '1' || caracter == '0' || caracter == 'x') {
                ImgCampo[f][c] = caracter; // Asignar el caracter a la matriz
            } 
            else if ( caracter == '\n') 
            {
                c--;
            }
        }
    }

   //Libero la memoria contenida en archivo
      archivo.close();

    
  cout<<"Texto de entrada:\n"<<endl;
    for (int f = 0; f < MAX_PIXLS; f++)
    {
        for (int c = 0; c < MAX_PIXLS; c++)
        {  
            cout << ImgCampo[f][c];
        }
        cout <<endl;
    }
       

  cout << "\n------------------------------\n" << endl;


    // Crearemos una copia de la imagen que sea modificable
    // con los datos obtenidos y recortando los puntos vacios
    char ImgMod[MAX_PIXLS][MAX_PIXLS];

     for (int f = 0 ; f < numeroFilas; f++)
        {
            for (int c = 0; c < numeroColumnas; c++)
            { ImgMod[f][c] = ImgCampo[f][c];}
        }

    // Plantaremos el drone en la posicion 1,1

    ImgMod[Drone().posX][Drone().posY] = Drone().formaDrone;
    
    // SE PUEDE CAMBIAR NUM FILAS Y COLS. POR MAX_PIXLS PARA MOSTRAR LA MATRIZ SIN RECORTAR
    for (int f = 0; f < numeroFilas; f++)
        {
            for (int c = 0; c < numeroColumnas; c++)
            {  
                cout << ImgMod[f][c];
            }
            cout <<endl; 
        }

    cout << endl;

    //Contadores
    int contadorMalezas = 0;
    int contadorBarrerasHorizontales = 0;
    int contadorBarrerasVerticales = 0;

    cout<< "\n Posicion del centro de Malezas Importantes"<< endl;
    Maleza* malezas = filtroMaleza(ImgCampo, contadorMalezas); // Setea posicion de las malezas
    for(int i=0; i<contadorMalezas; i++){ 
        cout <<"posX"<< i << ": " << malezas[i].getPosX() << " " << "posY"<<i<<": " << malezas[i].getPosY() << endl; 
    }
    
    //Arrays con barreras
    Barrera* barrerasH = filtroBarreraHorizontal(ImgCampo, contadorBarrerasHorizontales);
    Barrera* barrerasV = filtroBarreraVertical(ImgCampo, contadorBarrerasVerticales);
    
    //Impresion de barreras
    cout<< "\n Barreras Horizontales"<< endl;
    for(int i=0; i<contadorBarrerasHorizontales; i++){ 
        cout << "nro" << i << ", " <<"posInicioX: " << barrerasH[i].getPosX() << " " << "posInicioY: " << barrerasH[i].getPosY() << " " << "Largo: " << barrerasH[i].getLargo() << endl;
    }
    
    cout<< "\n Barreras Verticales"<< endl;
    for(int i=0; i<contadorBarrerasVerticales; i++){ 
        cout << "nro" << i << ", " <<"posInicioX: " << barrerasV[i].getPosX() << " " << "posInicioY: " << barrerasV[i].getPosY() << " " << "Largo: " << barrerasV[i].getLargo() << endl;
    }

    int pesos[contadorMalezas][contadorMalezas];


    cout<<endl;
    cout<<"___________ Matriz de pesos ponderados __________"<<endl;

    for(int i = 0; i < contadorMalezas; i++){
        for(int j = 0; j < contadorMalezas; j++){
            pesos[i][j] = dist_malezas(malezas[i],malezas[j]);
        }
    }
    
    for(int i = 0; i < contadorMalezas; i++){
        for(int j = 0; j < contadorMalezas; j++){
            printf(" %5i", pesos[i][j]);
        }
        cout << endl;
    }

    cout<<"\n\n_________ Matriz de pesos ponderados con barreras __________"<<endl;

    for(int i=0; i<contadorMalezas; i++){
        for(int j=0; j<contadorMalezas; j++){
            for(int k=0; k<contadorBarrerasHorizontales; k++){
                if(malezaBarrera(malezas[i], malezas[j], barrerasH[k])){
                    pesos[i][j] = INF;
                }
            }
            for(int k=0; k<contadorBarrerasVerticales; k++){
                    if(malezaBarrera(malezas[i], malezas[j], barrerasV[k])){
                        pesos[i][j] = INF;
                    }
                }
        }
    }

    for(int i=0; i<contadorMalezas; i++){
        cout<<"\n";
        for(int j=0; j<contadorMalezas; j++){
            printf(" %5i", pesos[i][j]);
        }
    }


    cout<<"\n\n_______________ Matriz de caminos __________________"<<endl;
    int pesos1[V][V], pesos2[V][V]; // V const; pesos2 (copia para trabajarse)

    for(int i = 0; i < contadorMalezas; i++){
        for(int j = 0; j < contadorMalezas; j++){
            if(pesos[i][j] == INF || pesos[i][j] == 0)
                pesos1[i][j] = 0; // Si NO existe camino entre dos vertices
            else
                pesos1[i][j] = 1; // Si existe camino entre dos vertices
        }
    }

    for(int i=0; i<contadorMalezas; i++){
        cout<<"\n";
        for(int j=0; j<contadorMalezas; j++){
            printf(" %5i", pesos1[i][j]);
        }
    }

    cout<<"\n\n--------- Copia Matriz de pesos ponderados con barrera ---------"<<endl;
    for(int i = 0; i < contadorMalezas; i++){
        for(int j = 0; j < contadorMalezas; j++){
            pesos2[i][j] = pesos[i][j];
        }
    }

    for(int i=0; i<contadorMalezas; i++){
        cout<<"\n";
        for(int j=0; j<contadorMalezas; j++){
            printf(" %5i", pesos2[i][j]);
        }
    }
    
    cout<<"\n\n______________________________________________________________________"<<endl;
    cout<<"Ciclos Hamiltonianos disponibles:"<<endl;
    Cola *cola = new Cola();
    buscarHamilton(pesos1, contadorMalezas, pesos2, cola);

    cout<<"\n\n______________________________________________________________________"<<endl;

    cout<<"\nCola de vertices (camino a recorrer):"<<endl;
    cout<<endl;
    
    cola->imprimir("");
    cout << "\n"<< endl;


    if(cola->colavacia() == false){

        cout << "Drone despega de ";
        cout << "x: " << Drone().posX << " y: "<< Drone().posX << endl;
        cout << "Y comienza a dirigirse a las siguientes posiciones: \n" << endl;
        
        for(int i=0; i<contadorMalezas; i++){   
            cout << "Maleza nro "<< cola->tope() <<endl;
            cout << "x: " << malezas[cola->tope()].getPosX() << " y: "<<malezas[cola->tope()].getPosY() << endl;
            cola->desencolar();
            cout << "\n";
            if ( i == contadorMalezas-1) // Cuando llega a la ultima maleza indica que vuelva al punto de partida
            {  cout << "Vuelve al punto de partida " <<endl;
            cout << "x: " << Drone().posX << " y: "<< Drone().posX << endl; }
        }
    }
    else {
        cout<<"No hay camino disponible a recorrer"<<endl; 
    }

    system("PAUSE");
    return EXIT_SUCCESS;
}