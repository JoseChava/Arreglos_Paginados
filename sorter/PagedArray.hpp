#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//Estructura de una pagina del PagedArray, contiene el arreglo de enteros, numero de pagina (ID de la pagina) y un booleano para saber si se ha modificado
struct Pagina {
	int* informacion;
	int numeroPagina;
	bool modificado;
};

class PagedArray {
	private:
		//Variables privadas importantes para la clase PagedArray
		fstream archivo;
		Pagina* paginas;
		int tamanhoPagina;
		int paginasEnRam;
		int pageHits;
		int pageFaults;
public:
	PagedArray(string pathArchivo, int tamanhoPagina, int paginasEnRam); //constructor para inicializar el arreglo paginado
	~PagedArray(); //destructor para liberar la memoria del arreglo paginado
 
	int& operator[](int index); //Sobrecarga del operador []

	//Funciones basicas para PageHits y PageFaults
	int obtenerPageHits() const;
	int obtenerPageFaults() const;
};
