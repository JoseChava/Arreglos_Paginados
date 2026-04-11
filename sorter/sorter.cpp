#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include "PagedArray.hpp"
#include "../algoritmos_ordenamiento/Algoritmos.hpp"

using namespace std;

//Funcion para copiar el archivo creado con generator binario para poder manipularlo
void CopiarArchivo(string PathOriginal, string PathSalida) {
	ifstream archivoOriginal(PathOriginal, ios::binary);
	ofstream archivoSalida(PathSalida, ios::binary);
	//Buffer de 1MB para mejorar eficiencia en el copiado del archivo binario
	const int bufferTamanho = 1024 * 1024;
	char* bufferLectura = new char[bufferTamanho];
	char* bufferEscritura = new char[bufferTamanho];
	archivoOriginal.rdbuf()->pubsetbuf(bufferLectura, bufferTamanho);
	archivoSalida.rdbuf()->pubsetbuf(bufferEscritura, bufferTamanho);
	int numero;
	while (archivoOriginal.read((char*)&numero, sizeof(int))) { //El ciclo para una vez que NO se pueda leer otros 4 bytes (un int)
		archivoSalida.write((char*)&numero, sizeof(int));
	}
	archivoOriginal.close();
	archivoSalida.close();
	delete[] bufferLectura;
	delete[] bufferEscritura;
}

bool ArchivoExiste(string PathOriginal) {
	ifstream archivo(PathOriginal);
	return archivo.good();
}
//Generacion del archivo legible
void GenerarArchivoLegible(string PathBinario, string PathLegible) {
	//Se abre el binario desde el cual se copia al legible
	ifstream binario(PathBinario, ios::binary);
	ofstream legible(PathLegible);
	//Se crea un buffer de 1MB para mejorar la eficiencia del copiado a formato legible
	const int bufferTamanho = 1024 * 1024; 
	char* buffer = new char[bufferTamanho];
	//Se acumulan los numeros en el buffer para evitar hacer una escritura por cada numero 
	legible.rdbuf()->pubsetbuf(buffer, bufferTamanho); 
	int numero;
	bool primero = true;
	//Se lee el archivo y se copian los numeros separados por comas
	while (binario.read((char*)&numero, sizeof(int))) {
		if (!primero) legible << ',';
		legible << numero;
		primero = false;
	}
	//Se cierran ambos archivos al finalizar y se borra el buffer
	legible.close();
	binario.close();
	delete[] buffer;
}
int main(int argc, char* argv[]) {
	if (argc < 11) {
		cout << "Cantidad de parametros incorrectos";
		return 1;
	}
	string PathOriginal = argv[2];
	string PathSalida = argv[4];
	string AlgoritmoOrdenamiento = argv[6];

	if (string(argv[1]) != "-input") {
		cout << "Parametro -input incorrecto";
		return 1;
	}
	if (!ArchivoExiste(PathOriginal)) {
		cout << "No se encuentra el archivo: " << "'" << argv[2] << "'" << " Quiza no existe o se introdujo mal el nombre/path" << endl;
		return 1;
	}
	if (string(argv[3]) != "-output") {
		cout << "Parametro -output incorrecto";
		return 1;
	}
	if (string(argv[5]) != "-alg") {
		cout << "Parametro -alg incorrecto";
		return 1;
	}
	if (string(argv[7]) != "-pageSize") {
		cout << "Parametro -pageSize incorrecto";
		return 1;
	}
	if (string(argv[9]) != "-pageCount") {
		cout << "Parametro -pageCount incorrecto";
		return 1;
	}

	string valorPageSize = string(argv[8]);
	for (int i = 0; i < valorPageSize.length(); i++) {
		char c = valorPageSize[i];
		if (!isdigit(c)) {
			cout << "pageSize debe ser un numero entero positivo" << endl;
			return 1;
		}
	}
	string valorPageCount = string(argv[10]);
	for (int i = 0; i < valorPageCount.length(); i++) {
		char c = valorPageCount[i];
		if (!isdigit(c)) {
			cout << "pageCount debe ser un numero entero positivo" << endl;
			return 1;
		}
	}

	int TamanhoPagina = stoi(argv[8]);
	int PaginasEnRam = stoi(argv[10]);

	if ((TamanhoPagina & (TamanhoPagina - 1)) != 0) {
		cout << "pageSize debe ser potencia de 2" << endl;
		return 1;
	}

	if (AlgoritmoOrdenamiento!="RS"&&AlgoritmoOrdenamiento!="QS"&&
		AlgoritmoOrdenamiento!="MS"&&AlgoritmoOrdenamiento!="HS"&&
		AlgoritmoOrdenamiento!="CS") {
		cout << "Algoritmo invalido. Opciones validas: RS, CS, MS, HS, QS" << endl;
		return 1;
	}

	// Se restringen las entradas de tamaño para ciertos algoritmos
	{
		ifstream archivoValidacion(PathOriginal, ios::binary | ios::ate);
		long long tamanoMB = archivoValidacion.tellg() / (1024 * 1024);

		if (AlgoritmoOrdenamiento=="HS" && tamanoMB > 64) {
			cout << "Heap Sort no es viable para archivos mayores a 64MB (P4)" << endl;
			return 1;
		}
		if (AlgoritmoOrdenamiento=="CS" && tamanoMB > 128) {
			cout << "Comb Sort no es viable para archivos mayores a 128MB (P5)" << endl;
			return 1;
		}
		if (AlgoritmoOrdenamiento=="MS" && tamanoMB > 512) {
			cout << "Merge Sort no es viable para archivos mayores a 512MB (MEDIUM)" << endl;
			return 1;
		}
	}

	cout << "Copiando archivo..." << endl;
	CopiarArchivo(PathOriginal, PathSalida);
	cout << "Archivo copiado exitosamente." << endl;

	long long PageHits = 0;
	long long PageFaults = 0;
	chrono::duration<double> duracion;
	chrono::high_resolution_clock::time_point inicio, fin;
	string pathTemp = "";

	{
		PagedArray PagedArr(PathSalida, TamanhoPagina, PaginasEnRam);
		long long TamanhoArchivo = PagedArr.tamanhoArchivo();
		cout << "Ordenando archivo..." << endl;

		if (string(argv[6])=="RS") {
			string pathTemp=PathSalida + "_temp.bin";
			ofstream temp(pathTemp, ios::binary);
			temp.seekp((long long)TamanhoArchivo * sizeof(int) - 1);
			temp.write("", 1);
			temp.close();
			PagedArray outputRS(pathTemp,TamanhoPagina,PaginasEnRam);
			inicio=chrono::high_resolution_clock::now();
			radixSort(PagedArr, outputRS, TamanhoArchivo);
			fin=chrono::high_resolution_clock::now();
		}
		else if (string(argv[6])=="CS") {
			inicio = chrono::high_resolution_clock::now();
			combSort(PagedArr, PagedArr.tamanhoArchivo());
			fin = chrono::high_resolution_clock::now();
		}
		else if (string(argv[6])=="MS") {
			pathTemp=PathSalida+"_temp.bin";
			ofstream temp(pathTemp, ios::binary);
			temp.seekp((long long)TamanhoArchivo * sizeof(int) - 1);
			temp.write("", 1);
			temp.close();
			PagedArray auxMS(pathTemp,TamanhoPagina,PaginasEnRam);
			inicio=chrono::high_resolution_clock::now();
			mergeSort(PagedArr,auxMS,TamanhoArchivo);
			fin =chrono::high_resolution_clock::now();
		}
		else if (string(argv[6])=="HS") {
			inicio = chrono::high_resolution_clock::now();
			heapSort(PagedArr, PagedArr.tamanhoArchivo());
			fin = chrono::high_resolution_clock::now();
		}
		else if (string(argv[6])=="QS") {
			inicio = chrono::high_resolution_clock::now();
			quickSort(PagedArr, 0, PagedArr.tamanhoArchivo() - 1);
			fin = chrono::high_resolution_clock::now();
		}

		PageHits = PagedArr.obtenerPageHits();
		PageFaults = PagedArr.obtenerPageFaults();
		duracion = fin - inicio;
	}

	if (!pathTemp.empty())
		remove(pathTemp.c_str());

	string nombreAlgoritmo;
	if (AlgoritmoOrdenamiento == "RS") nombreAlgoritmo = "Radix Sort";
	else if (AlgoritmoOrdenamiento == "QS") nombreAlgoritmo = "Quick Sort";
	else if (AlgoritmoOrdenamiento == "MS") nombreAlgoritmo = "Merge Sort";
	else if (AlgoritmoOrdenamiento == "HS") nombreAlgoritmo = "Heap Sort";
	else if (AlgoritmoOrdenamiento == "CS") nombreAlgoritmo = "Comb Sort";

	cout << "Archivo ordenado exitosamente." << endl;
	cout << "========= Resultados =========" << endl;
	cout << "Algoritmo:          " << nombreAlgoritmo << endl;
	cout << "Tiempo de ordenado: " << duracion.count() << " segundos" << endl;
	cout << "Page Hits:          " << PageHits << endl;
	cout << "Page Faults:        " << PageFaults << endl;
	cout << "==============================" << endl;

	cout << "Creando copia legible..." << endl;
	GenerarArchivoLegible(PathSalida, PathSalida + ".txt");
	cout << "Copia legible creada exitosamente." << endl;

	return 0;
}
