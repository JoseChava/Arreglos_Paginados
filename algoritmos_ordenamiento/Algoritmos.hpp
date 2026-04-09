
#include "../sorter/PagedArray.hpp"
//Funciones comunes a los algoritmos implementadas a mano
void Swap(int& a, int& b);
int Min(int a, int b);
//Funciones para Comb Sort
void combSort(PagedArray& a, int n);
int getNextGap(int gap);
//Funciones para Marge Sort
void merge(PagedArray& arr, PagedArray& aux, int left, int mid, int right);
void mergeSort(PagedArray& arr, PagedArray& aux, int n);
//Funciones para Heap Sort
void heapify(PagedArray& arr, int n, int i);
void heapSort(PagedArray& arr, int n);
//Funciones para Quick Sort
int partition(PagedArray& arr, int low, int high);
void quickSort(PagedArray& arr, int low, int high);
//Funciones para Radix Sort
void countingSort(PagedArray& array, PagedArray& output, int size, int shift);
void radixSort(PagedArray& array, PagedArray& output, int size);