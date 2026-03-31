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

void GenerarArchivoLegible(string PathBinario, string PathLegible) {
	//Se abre el archivo binario ordenado anteriormente y se crea uno nuevo legible donde se escribiran los numeros en formato ASCII
	ifstream binario(PathBinario, ios::binary);
	ofstream legible(PathLegible);
	int numero;
	//Se recorre el archivo binario en saltos de 4 Bytes para leer cada numero 
	for (int i = 0; binario.read((char*)&numero, sizeof(int)); i++) {
		//Si es el primer numero leido, no se escribe la coma para que se empieze siempre por el primer int
		if (i > 0) {
			//Se escribe la coma espaciadora
			legible << ",";
		}
		//Se escribe el numero en formato ASCII al archivo legible
		legible << numero;
	}
	//Se cierran ambos archivos
	binario.close();
	legible.close();
}
//Algoritmos de ordenamiento

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

int partition(PagedArray& arr, int first, int last)
{
	int pivot = arr[last];
	int i = first;
	for (int j = first; j < last; j++) {
		if (arr[j] <= pivot) {
			swap(arr[i], arr[j]);
			i++;
		}
	}
	swap(arr[i], arr[last]);
	return (i);
}

void quickSort(PagedArray& arr, int first, int last)
{
	int* stack = new int[last - first + 1];
	int top = -1;
	stack[++top] = first;
	stack[++top] = last;
	while (top >= 0) {
		last = stack[top--];
		first = stack[top--];
		int pivot_pos = partition(arr, first, last);
		if (pivot_pos - 1 > first) {
			stack[++top] = first;
			stack[++top] = pivot_pos - 1;
		}
		if (pivot_pos + 1 < last) {
			stack[++top] = pivot_pos + 1;
			stack[++top] = last;
		}
	}
	delete[] stack;
}

int getMax(int array[], int n) {
	int max = array[0];
	for (int i = 1; i < n; i++)
		if (array[i] > max)
			max = array[i];
	return max;
}

void countingSort(PagedArray& array, int size, int place) {
	const int max = 10;
	int* output = new int[size];
	int count[max];

	for (int i = 0; i < max; ++i)
		count[i] = 0;

	for (int i = 0; i < size; i++)
		count[(array[i] / place) % 10]++;

	for (int i = 1; i < max; i++)
		count[i] += count[i - 1];

	for (int i = size - 1; i >= 0; i--) {
		output[count[(array[i] / place) % 10] - 1] = array[i];
		count[(array[i] / place) % 10]--;
	}

	for (int i = 0; i < size; i++)
		array[i] = output[i];

	delete[] output;
}

void radixSort(PagedArray& array, int size) {
	int max = array[0];
	for (int i = 1; i < size; i++)
		if (array[i] > max)
			max = array[i];

	for (int place = 1; max / place > 0; place *= 10)
		countingSort(array, size, place);
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

	long long PageHits = 0;
	long  long PageFaults = 0;
	chrono::duration<double> duracion; 
	chrono::steady_clock::time_point inicio, fin;

	{
		//Se crea el objeto tipo PagedArray
		PagedArray PagedArr(PathSalida, TamanhoPagina, PaginasEnRam);
		//Se ejecuta un algoritmo de ordenamiento segun el que se elija en la linea de comandos
		if (string(argv[6]) == "RS") {
			inicio = chrono::high_resolution_clock::now();
			radixSort(PagedArr, PagedArr.tamanhoArchivo());
			fin = chrono::high_resolution_clock::now();

		}
		else if (string(argv[6]) == "SS") {
			inicio = chrono::high_resolution_clock::now();
			shellSort(PagedArr, PagedArr.tamanhoArchivo());
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
			cout << "Parametro de Algoritmod e ordenameinto incorrecto";
			cout << "Opciones validas: 'IS':InsertionSort 'SS': ShellSort 'MS':MergeSort 'HS':HeapSort y 'QS':QuickSort";
			return 1;
		}
		//Se obtienen los datos importantes para el mensaje de resultados
		PageHits = PagedArr.obtenerPageHits();
		PageFaults = PagedArr.obtenerPageFaults();
		duracion = fin - inicio;
	}
	cout << "Resultados:" << endl;
	cout << "Algoritmo:    " << AlgoritmoOrdenamiento << endl;
	cout << "Tiempo:       " << duracion.count() << " segundos" << endl;
	cout << "Page Hits:    " << PageHits << endl;
	cout << "Page Faults:  " << PageFaults << endl;

	GenerarArchivoLegible(PathSalida, PathSalida+".txt");

	return 0;
}


