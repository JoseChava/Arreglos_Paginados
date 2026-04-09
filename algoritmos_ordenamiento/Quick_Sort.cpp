#include "Algoritmos.hpp"
#include "../sorter/PagedArray.hpp"

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