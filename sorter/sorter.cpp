#include <iostream>
#include <string>
#include <fstream>
#include "PagedArray.hpp"

using namespace std;

//Funcion para copiar el archivo creado con generator binario para poder manipularlo
void CopiarArchivo(string PathOriginal, string PathSalida) {
	ifstream archivoOriginal(PathOriginal, ios::binary);
	ofstream archivoSalida(PathSalida, ios::binary);
	int numero;
	while (archivoOriginal.read((char*)&numero, sizeof(int))) { //El ciclo para una vez que NO se pueda leer otros 4 bytes (un int)
		archivoSalida.write((char*)&numero, sizeof(int));
	}
	archivoOriginal.close();
	archivoSalida.close();
}

bool ArchivoExiste(string PathOriginal) {
	ifstream archivo(PathOriginal);
	return archivo.good();
}

void insertionSort(PagedArray& arr, int n)
{
	for (int i = 1; i < n; ++i) {
		int key = arr[i];
		int j = i - 1;
		while (j >= 0 && arr[j] > key) {
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = key;
	}
}

void shellSort(PagedArray& arr, int n) {
	for (int interval = n / 2; interval > 0; interval /= 2) {
		for (int i = interval; i < n; i += 1) {
			int temp = arr[i];
			int j;
			for (j = i; j >= interval && arr[j - interval] > temp; j -= interval) {
				arr[j] = arr[j - interval];
			}
			arr[j] = temp;
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
	int pivot = arr[high]; 
	int i = (low - 1);  
	for (int j = low; j <= high - 1; j++) {
		if (arr[j] < pivot) {
			i++;
			std::swap(arr[i], arr[j]);
		}
	}
	std::swap(arr[i + 1], arr[high]);
	return (i + 1);
}

void quickSort(PagedArray& arr, int low, int high) {
	if (low < high) {
		int pi = partition(arr, low, high);
		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
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
	if (string(argv[0]) != "sorter") {
		cout << "Nombre del proyecto incorrecto";
		return 1;
	}
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
	CopiarArchivo(PathOriginal, PathSalida);
	//Se crea el objeto tipo PagedArray
	PagedArray PagedArr(PathSalida, TamanhoPagina, PaginasEnRam);
	//Se ejecuta un algoritmo de ordenamiento segun el que se elija en la linea de comandos
	if (string(argv[6]) == "IS") {
		insertionSort(PagedArr, PagedArr.tamanhoArchivo()-1);

	}
	else if (string(argv[6]) == "SS") {
		shellSort(PagedArr, PagedArr.tamanhoArchivo()-1);

	}
	else if (string(argv[6]) == "MS") {
		mergeSort(PagedArr, PagedArr.tamanhoArchivo()-1);

	}
	else if (string(argv[6]) == "HS") {
		heapSort(PagedArr, PagedArr.tamanhoArchivo()-1);

	}
	else if (string(argv[6]) == "QS") {
		quickSort(PagedArr,0,PagedArr.tamanhoArchivo()-1);

	}
	else {
		cout << "Parametro de Algoritmod e ordenameinto incorrecto";
		cout << "Opciones validas: 'IS':InsertionSort 'SS': ShellSort 'MS':MergeSort 'HS':HeapSort y 'QS':QuickSort";
	}

	return 0;
}


