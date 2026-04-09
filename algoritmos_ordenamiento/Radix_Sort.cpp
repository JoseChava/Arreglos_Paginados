#include "Algoritmos.hpp"
#include "../sorter/PagedArray.hpp"

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
void radixSort(PagedArray& array, PagedArray& output, int size) {
	for (int shift = 0; shift < 32; shift += 8)
		countingSort(array, output, size, shift);
}