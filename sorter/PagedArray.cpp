#include <iostream>
#include <fstream>
#include "PagedArray.hpp"

using namespace std;

PagedArray::PagedArray(string pathArchivo, int tamanhoPagina, int paginasEnRam) {
	//Variables vitales para el arreglo paginado
	this->tamanhoPagina = tamanhoPagina;
	this->paginasEnRam = paginasEnRam;
	this->pageHits = 0;
	this->pageFaults = 0;
	this->reloj = 0;
	this->archivo.open(pathArchivo, ios::in | ios::out | ios::binary); //Se abre para poder escribir y leer del archivo
	this->paginas = new Pagina[paginasEnRam];//Arreglo de paginas, simula la memoria RAM
	this->cantidadPaginas = tamanhoArchivo() / tamanhoPagina + 1; //Paginas que hay en todo el PagedArray
	this->tablaIndices = new int[cantidadPaginas]; //Se crea la lista de indices de espacios en RAM para acceso rapido
	//Se inicializan todos los indices a -1 porque al inicio ninguna pagina esta en RAM
	for (int i = 0; i < cantidadPaginas; i++) {
		tablaIndices[i] = -1;
	}
	for (int i = 0; i < paginasEnRam; i++) {
		//Se inicializan las paginas del Pagedrray y se meten dentro del arreglo de paginas
		paginas[i].informacion = new int[tamanhoPagina]; //Arreglo de numeros (pagina)
		paginas[i].numeroPagina = -1;
		paginas[i].modificado = false;
		paginas[i].contador = 0;
	}
}
//Destructor para liberar la memoria del arreglo paginado
	PagedArray::~PagedArray() {
		for (int i = 0; i < paginasEnRam; i++) {
			if (paginas[i].numeroPagina!=-1 && paginas[i].modificado == true) {
				archivo.seekp(paginas[i].numeroPagina * tamanhoPagina * sizeof(int),ios::beg);
				archivo.write((char*)paginas[i].informacion, tamanhoPagina * sizeof(int));
			}
			delete[] paginas[i].informacion;
		}
		delete[] paginas;
		delete[] tablaIndices;
		archivo.close();
	}
	//Implementacion de funciones para obtener los hits y faults
	long long PagedArray::obtenerPageHits() const {
		return pageHits;
	}
	long long PagedArray::obtenerPageFaults() const {
		return pageFaults;
	}
	//Funcion para calcular el tamanho total en int que tiene el archivo ordenar, luego esto se pasa a los algoritmos de ordenamiento
	long long PagedArray::tamanhoArchivo() {
		//se posiciona el puntero en el final del archivo
		archivo.seekg(0, ios::end);
		//Se obtiene la posicion actual desde el inicio, que es igual que la cantidad de Bytes
		long long tamanho = archivo.tellg();
		//Se transforma el tamanho en bytes a ints
		return tamanho / sizeof(int);
	}
	//Sobrecarga del operador [] para poder psarlo como parametro a los algoritmo de ordenamiento y buscar valores como en un array normal
	int& PagedArray::operator[](int indice) {
		int numeroPagina = indice / tamanhoPagina;
		int desfase = indice % tamanhoPagina;
		//Se busca en la tabla de indices a ver si la pagina que se busca esta o no en RAM (si es -1 NO esta en RAM)
		if (tablaIndices[numeroPagina] == -1) {
			//Se llama un PageFault y se caraga la pagina del disco pues no estaba en RAM
			pageFaults++;
			cargarPagina(numeroPagina);
		}
		else {
			//Caso contrario hay pageHit
			pageHits++;
		}
		//Se define el indice de la RAM como el valor encontrado en la tabla de indices al buscar en la posicion del numero de pagina
		int indiceRam = tablaIndices[numeroPagina];
		//Se configuran las caracteristicas de la pagina para saber que ah sido accedida y se actualiza el reloj para saber que fue la ultima accedida
		paginas[indiceRam].contador = reloj;
		paginas[indiceRam].modificado = true;
		reloj++;
		return paginas[indiceRam].informacion[desfase];
	}
	//Se encarga de cargar la pagina solicitada del archivo, ademas, guarda los cambios de una pagina marcada como modificada si esta es intercambiada por la solicitada
		void PagedArray::cargarPagina(int numeroPagina) {
			//Se busca si hay espacios marcados como vacios en el array de paginas, si hay se copia la informaicon del archivo a dicha pagina
			for (int i = 0; i < paginasEnRam; i++) {
				if (paginas[i].numeroPagina == -1) {
					paginas[i].numeroPagina = numeroPagina;
					tablaIndices[numeroPagina] = i;
					paginas[i].modificado = false;
					paginas[i].contador = reloj;
					reloj++;
					archivo.seekg(numeroPagina * tamanhoPagina * sizeof(int), ios::beg);
					archivo.read((char*)paginas[i].informacion, tamanhoPagina * sizeof(int));
					return;
				}
			}
			//Si no hay espacios vacios se usa LRU para intercambiar la pagina menos usada por la pagina solicitada
			int minContador = paginas[0].contador;
			int pagACambiar = 0;
			for (int i = 0; i < paginasEnRam; i++) {
				//Se selecciona la pagina con el contador mas bajo (LRU)
				if (paginas[i].contador < minContador) {
					minContador = paginas[i].contador;
					pagACambiar = i;
				}
			}
			//Si la pagina esta marcada como modificada, se guardan sus cambios en el archivo
			if (paginas[pagACambiar].modificado == true) {
				archivo.seekp(paginas[pagACambiar].numeroPagina * tamanhoPagina * sizeof(int), ios::beg);
				archivo.write((char*)paginas[pagACambiar].informacion, tamanhoPagina * sizeof(int));
			}
			tablaIndices[paginas[pagACambiar].numeroPagina] = -1;
			//Se copian los datos necesarios a la nueva pagina
			archivo.seekg(numeroPagina * tamanhoPagina * sizeof(int), ios::beg);
			archivo.read((char*)paginas[pagACambiar].informacion, tamanhoPagina * sizeof(int));
			//Se asignan los datos predeterminados a la nueva pagina
			paginas[pagACambiar].numeroPagina = numeroPagina;
			tablaIndices[numeroPagina] = pagACambiar;
			paginas[pagACambiar].contador = reloj;
			reloj++;
			paginas[pagACambiar].modificado = false;
		}
