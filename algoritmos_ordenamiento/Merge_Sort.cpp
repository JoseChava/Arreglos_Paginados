#include "Algoritmos.hpp"
#include "../sorter/PagedArray.hpp"

void merge(PagedArray& arr, PagedArray& aux, int left, int mid, int right) {
	for (int i = left; i <= mid; i++)
		aux[i] = arr[i];

	int i = left, j = mid + 1, k = left;
	while (i <= mid && j <= right) {
		if (aux[i] <= arr[j])
			arr[k++] = aux[i++];
		else
			arr[k++] = arr[j++];
	}
	while (i <= mid)
		arr[k++] = aux[i++];
}

int Min(int a, int b) {
	if (a < b) {
		return a;
	}
	return b;
}
void mergeSort(PagedArray& arr, PagedArray& aux, int n) {
	for (int currSize = 1; currSize <= n - 1; currSize = 2 * currSize) {
		for (int leftStart = 0; leftStart < n - 1; leftStart += 2 * currSize) {
			int mid = Min(leftStart + currSize - 1, n - 1);
			int rightEnd = Min(leftStart + 2 * currSize - 1, n - 1);
			merge(arr, aux, leftStart, mid, rightEnd);
		}
	}
}