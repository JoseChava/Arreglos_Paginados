#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include "PagedArray.hpp"

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
//Algoritmos de ordenamiento 
int getNextGap(int gap)
{
	gap = (gap * 10) / 13;

	if (gap < 1)
		return 1;
	return gap;
}
void Swap(int& a, int& b) {
	int temp = a;
	a=b;
	b=temp;
}
void combSort(PagedArray& a, int n)
{
	int gap = n;
	bool swapped = true;
	while (gap != 1 || swapped == true)
	{
		gap = getNextGap(gap);
		swapped = false;
		for (int i = 0; i < n - gap; i++)
		{
			if (a[i] > a[i + gap])
			{
				Swap(a[i], a[i + gap]);
				swapped = true;
			}
		}
	}
}

void merge(PagedArray& arr, PagedArray& aux, int left, int mid, int right) {
	// Solo copiar mitad izquierda a aux
	for (int i = left; i <= mid; i++)
		aux[i] = arr[i];

	int i = left, j = mid + 1, k = left;
	while (i <= mid && j <= right) {
		if (aux[i] <= arr[j])
			arr[k++] = aux[i++];
		else
			arr[k++] = arr[j++];
	}
	// Copiar lo que quede de la izquierda
	while (i <= mid)
		arr[k++] = aux[i++];
	// La derecha ya esta en su lugar, no hay que copiar
}

int Min(int a, int b) {
	if (a < b) {
		return a;
	}
	return b;
}

void mergeSort(PagedArray& arr, int n, int tamanhoPagina, int paginasEnRam) {
	// Crear archivo auxiliar una sola vez
	string pathTemp = "merge_temp.bin";
	ofstream temp(pathTemp, ios::binary);
	int cero = 0;
	for (long long i = 0; i < n; i++)
		temp.write((char*)&cero, sizeof(int));
	temp.close();

	PagedArray aux(pathTemp, tamanhoPagina, paginasEnRam);

	for (int currSize = 1; currSize <= n - 1; currSize = 2 * currSize) {
		for (int leftStart = 0; leftStart < n - 1; leftStart += 2 * currSize) {
			int mid = Min(leftStart + currSize - 1, n - 1);
			int rightEnd = Min(leftStart + 2 * currSize - 1, n - 1);
			merge(arr, aux, leftStart, mid, rightEnd);
		}
	}

	remove(pathTemp.c_str());
}

void heapify(PagedArray& arr, int n, int i) {
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < n && arr[left] > arr[largest])
		largest = left;

	if (right < n && arr[right] > arr[largest])
		largest = right;
	if (largest != i) {
		Swap(arr[i], arr[largest]);
		heapify(arr, n, largest);
	}
}
void heapSort(PagedArray& arr, int n) {
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);
	for (int i = n - 1; i >= 0; i--) {
		Swap(arr[0], arr[i]);
		heapify(arr, i, 0);
	}
}

int partition(PagedArray& arr, int low, int high) {
	int pivot = arr[low];
	int i = low - 1, j = high + 1;
	while (true) {
		do {
			i++;
		} while (arr[i] < pivot);
		do {
			j--;
		} while (arr[j] > pivot);
		if (i >= j)
			return j;
		Swap(arr[i], arr[j]);
	}
}

void quickSort(PagedArray& arr, int low, int high) {
	if (low < high) {

		int pi = partition(arr, low, high);
		quickSort(arr, low, pi);
		quickSort(arr, pi + 1, high);
	}
} 

void countingSort(PagedArray& array, PagedArray& output, int size, int shift) {
	const int BASE = 256;
	int count[BASE] = { 0 };

	for (int i = 0; i < size; i++)
		count[((unsigned int)(int)array[i] >> shift) & 0xFF]++;

	int acumulado = 0;
	for (int i = 0; i < BASE; i++) {
		int c = count[i];
		count[i] = acumulado;
		acumulado += c;
	}
	for (int i = 0; i < size; i++) {
		int byte = ((unsigned int)(int)array[i] >> shift) & 0xFF;
		output[count[byte]++] = array[i];
	}

	for (int i = 0; i < size; i++)
		array[i] = output[i];
}

void radixSort(PagedArray& array, int size, int tamanhoPagina, int paginasEnRam) {
	// Se crea archivo temporal para usar pagedArray
	string pathTemp = "output_temp.bin";
	ofstream temp(pathTemp, ios::binary);
	int cero = 0;
	for (int i = 0; i < size; i++)
		temp.write((char*)&cero, sizeof(int));
	temp.close();

	PagedArray output(pathTemp, tamanhoPagina, paginasEnRam);

	for (int shift = 0; shift < 32; shift += 8)
		countingSort(array, output, size, shift);

	remove(pathTemp.c_str());
}
int main(int argc, char* argv[]) {
	if (argc < 11) {
		cout << "Cantidad de parametros incorrectos";
		return 1;
	}
	string PathOriginal = argv[2];
	string PathSalida = argv[4];
	string AlgoritmoOrdenamiento = argv[6];
	int  TamanhoPagina = stoi(argv[8]);
	int PaginasEnRam = stoi(argv[10]);
	if (string(argv[1]) != "-input") {
		cout << "Parametro -input incorrecto";
		return 1;
	}
	if (!ArchivoExiste(PathOriginal)) {
		cout << "No se encuentra el archivo";
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
		cout << "Ordenando archivo..." << endl;

		if (string(argv[6]) == "RS") {
			pathTemp = PathSalida + "_temp.bin";
			inicio = chrono::high_resolution_clock::now();
			radixSort(PagedArr, PagedArr.tamanhoArchivo(), TamanhoPagina, PaginasEnRam);
			fin = chrono::high_resolution_clock::now();
		}
		else if (string(argv[6]) == "CS") {
			inicio = chrono::high_resolution_clock::now();
			combSort(PagedArr, PagedArr.tamanhoArchivo());
			fin = chrono::high_resolution_clock::now();
		}
		else if (string(argv[6]) == "MS") {
			inicio = chrono::high_resolution_clock::now();
			mergeSort(PagedArr, PagedArr.tamanhoArchivo(), TamanhoPagina, PaginasEnRam);
			fin = chrono::high_resolution_clock::now();
		}
		else if (string(argv[6]) == "HS") {
			inicio = chrono::high_resolution_clock::now();
			heapSort(PagedArr, PagedArr.tamanhoArchivo());
			fin = chrono::high_resolution_clock::now();
		}
		else if (string(argv[6]) == "QS") {
			inicio = chrono::high_resolution_clock::now();
			quickSort(PagedArr, 0, PagedArr.tamanhoArchivo() - 1);
			fin = chrono::high_resolution_clock::now();
		}
		else {
			cout << "Parametro de Algoritmo de ordenamiento incorrecto";
			cout << "Opciones validas: RS, CS, MS, HS, QS";
			return 1;
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
