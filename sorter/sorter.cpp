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
				swap(a[i], a[i + gap]);
				swapped = true;
			}
		}
	}
}

void merge(PagedArray& arr, int left, int mid, int right) {
	int n1 = mid - left + 1;
	int n2 = right - mid;

	int* arr1 = new int[n1];
	int* arr2 = new int[n2];

	for (int i = 0; i < n1; i++)
		arr1[i] = arr[left + i];
	for (int j = 0; j < n2; j++)
		arr2[j] = arr[mid + 1 + j];

	int i = 0;
	int j = 0;
	int k = left;

	while (i < n1 && j < n2) {
		if (arr1[i] <= arr2[j]) {
			arr[k] = arr1[i];
			i++;
		}
		else {
			arr[k] = arr2[j];
			j++;
		}
		k++;
	}

	while (i < n1) {
		arr[k] = arr1[i];
		i++;
		k++;
	}

	while (j < n2) {
		arr[k] = arr2[j];
		j++;
		k++;
	}

	delete[] arr1;
	delete[] arr2;
}

void mergeSort(PagedArray& arr, int n) {
	for (int currSize = 1; currSize <= n - 1; currSize = 2 * currSize) {
		for (int leftStart = 0; leftStart < n - 1; leftStart += 2 * currSize) {
			int mid = min(leftStart + currSize - 1, n - 1);
			int rightEnd = min(leftStart + 2 * currSize - 1, n - 1);
			merge(arr, leftStart, mid, rightEnd);
		}
	}
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
		swap(arr[i], arr[largest]);
		heapify(arr, n, largest);
	}
}
void heapSort(PagedArray& arr, int n) {
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);
	for (int i = n - 1; i >= 0; i--) {
		swap(arr[0], arr[i]);
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
		swap(arr[i], arr[j]);
	}
}

void quickSort(PagedArray& arr, int low, int high) {
	if (low < high) {

		int pi = partition(arr, low, high);
		quickSort(arr, low, pi);
		quickSort(arr, pi + 1, high);
	}
} 

void radixSort(PagedArray& array, int size) {
	const int BASE = 256;
	const int PASADAS = 4;
	int* buffer = new int[size];
	int* temp = new int[size];
	for (int i = 0; i < size; i++)
		buffer[i] = array[i];
	for (int pasada = 0; pasada < PASADAS; pasada++) {
		int conteo[BASE] = { 0 };
		int desplazamiento = pasada * 8;

		for (int i = 0; i < size; i++)
			conteo[((unsigned int)buffer[i] >> desplazamiento) & 0xFF]++;

		int acumulado = 0;
		for (int i = 0; i < BASE; i++) {
			int c = conteo[i];
			conteo[i] = acumulado;
			acumulado += c;
		}

		for (int i = 0; i < size; i++) {
			int indice = ((unsigned int)buffer[i] >> desplazamiento) & 0xFF;
			temp[conteo[indice]++] = buffer[i];
		}

		int* aux = buffer;
		buffer = temp;
		temp = aux;
	}
	for (int i = 0; i < size; i++)
		array[i] = buffer[i];

	delete[] buffer;
	delete[] temp;
}
int main(int argc, char* argv[]) {
	//Se definen las validaciones de la linea de comandos
	if (argc < 11) {
		cout << "Cantidad de parametros incorrectos";
		return 1;
	}
	string PathOriginal= argv[2];
	string PathSalida= argv[4];
	string AlgoritmoOrdenamiento= argv[6];
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
	long  long PageFaults = 0;
	chrono::duration<double> duracion; 
	chrono::high_resolution_clock::time_point inicio, fin;

	{
		//Se crea el objeto tipo PagedArray
		PagedArray PagedArr(PathSalida, TamanhoPagina, PaginasEnRam);
		//Se ejecuta un algoritmo de ordenamiento segun el que se elija en la linea de comandos
		cout << "Ordenando archivo..." << endl;
		if (string(argv[6]) == "RS") {
			inicio = chrono::high_resolution_clock::now();
			radixSort(PagedArr, PagedArr.tamanhoArchivo());
			fin = chrono::high_resolution_clock::now();

		}
		else if (string(argv[6]) == "CS") {
			inicio = chrono::high_resolution_clock::now();
			combSort(PagedArr, PagedArr.tamanhoArchivo());
			fin = chrono::high_resolution_clock::now();

		}
		else if (string(argv[6]) == "MS") {
			inicio = chrono::high_resolution_clock::now();
			mergeSort(PagedArr, PagedArr.tamanhoArchivo());
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
			cout << "Parametro de Algoritmo de ordenameinto incorrecto";
			cout << "Opciones validas: 'IS':InsertionSort 'SS': ShellSort 'MS':MergeSort 'HS':HeapSort y 'QS':QuickSort";
			return 1;
		}
		//Se obtienen los datos importantes para el mensaje de resultados
		PageHits = PagedArr.obtenerPageHits();
		PageFaults = PagedArr.obtenerPageFaults();
		duracion = fin - inicio;
	}
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


