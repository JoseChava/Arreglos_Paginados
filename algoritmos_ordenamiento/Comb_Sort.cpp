#include "Algoritmos.hpp"
#include "../sorter/PagedArray.hpp"

int getNextGap(int gap)
{
	gap = (gap * 10) / 13;
	if (gap < 1)
		return 1;
	return gap;
}
void Swap(int& a, int& b) {
	int temp = a;
	a = b;
	b = temp;
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