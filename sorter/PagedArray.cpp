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
	this->archivo.open(pathArchivo, ios::in | ios::out | ios::binary); //Se abre para poder escribir y leer del archivo
	this->paginas = new Pagina[paginasEnRam];//Arreglo de paginas, simula la memoria RAM
	for (int i = 0; i < paginasEnRam; i++) {
		//Se inicializan las paginas del Pagedrray y se meten dentro del arreglo de paginas
		paginas[i].informacion = new int[tamanhoPagina]; //Arreglo de numeros (pagina)
		paginas[i].numeroPagina = -1;
		paginas[i].modificado = false;
	}
}
//Destructor para liberar la memoria del arreglo paginado
	PagedArray::~PagedArray() {
		for (int i = 0; i < paginasEnRam; i++) {
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