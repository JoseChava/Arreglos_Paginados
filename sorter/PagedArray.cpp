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
		archivo.close();
	}
	//Implementacion de funciones para obtener los hits y faults
	int PagedArray::obtenerPageHits() const {
		return pageHits;
	}
	int PagedArray::obtenerPageFaults() const {
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
		//Se recorre array de paginas para verificar si la pagina solicitada se encuentra en RAM
		for (int i = 0; i < paginasEnRam; i++) {
			if (paginas[i].numeroPagina == numeroPagina) {
				pageHits++;
				paginas[i].contador = reloj;
				paginas[i].modificado = true;
				reloj++;
				return paginas[i].informacion[desfase];
			}
		}
		//Si no se encontro la pagina, se marca el Page Fault y se llama a cargarPagina
			pageFaults++;
			cargarPagina(numeroPagina);
			//Una vez cargada la pagina se vuelve a recorrer y se retorna el dato especifico pedido de la pagina
			for (int i = 0; i < paginasEnRam; i++) {
				if (paginas[i].numeroPagina == numeroPagina) {
					paginas[i].contador = reloj; 
					paginas[i].modificado = true;
					reloj++;
					return paginas[i].informacion[desfase];
				}
			}
		}
	//Se encarga de cargar la pagina solicitada del archivo, ademas, guarda los cambios de una pagina marcada como modificada si esta es intercambiada por la solicitada
		void PagedArray::cargarPagina(int numeroPagina) {
			//Se busca si hay espacios marcados como vacios en el array de paginas, si hay se copia la informaicon del archivo a dicha pagina
			for (int i = 0; i < paginasEnRam; i++) {
				if (paginas[i].numeroPagina == -1) {
					paginas[i].numeroPagina = numeroPagina;
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
			//Si no fue modificada, se copian los datos necesarios a la nueva pagina
			archivo.seekg(numeroPagina * tamanhoPagina * sizeof(int), ios::beg);
			archivo.read((char*)paginas[pagACambiar].informacion, tamanhoPagina * sizeof(int));
			//Se asignan los datos predeterminados a la nueva pagina
			paginas[pagACambiar].numeroPagina = numeroPagina;
			paginas[pagACambiar].contador = reloj;
			reloj++;
			paginas[pagACambiar].modificado = false;
		}
