#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

double W; 
int n; 
double *w; 
double *p;

int heap_limit; 

typedef struct
{
	int level;
	int *inORnot;
	double boundary;
	double profit;
	double weight;
	
}basket;

typedef struct
{
	basket *s;
	int heap_size;

}heap;

void shift_down(heap *H, int i)
{
	basket shiftkey;
	int parent;
	int found;
	int larger_child;

	shiftkey = H->s[i];
	parent = i;
	found = 0;
	while ((2 * parent <= H->heap_size) && !found)
	{
		if ((2 * parent < H->heap_size) && (H->s[2 * parent].boundary< H->s[2 * parent + 1].boundary))
			larger_child = 2 * parent + 1;
		else
			larger_child = 2 * parent;
		if (shiftkey.boundary < H->s[larger_child].boundary)
		{
			H->s[parent] = H->s[larger_child];
			parent = larger_child;
		}
		else
			found = 1;
	}
	H->s[parent] = shiftkey;
}

void insert(heap *HEAP, basket *newnode)
{
	int parent;
	heap temp_heap; 
	HEAP->heap_size = HEAP->heap_size + 1;
	if (HEAP->heap_size == heap_limit) 
	{
		heap_limit = heap_limit * 2;
		temp_heap.s = HEAP->s;
		HEAP->s = (basket *)malloc(sizeof(basket)*heap_limit);
		memcpy(HEAP->s, temp_heap.s, sizeof(basket)*(heap_limit / 2));
		free(temp_heap.s);
	}
	HEAP->s[HEAP->heap_size].inORnot = (int*)malloc(sizeof(int)*n);
	memcpy(HEAP->s[HEAP->heap_size].inORnot, newnode->inORnot, sizeof(int)*n);
	HEAP->s[HEAP->heap_size].boundary = newnode->boundary;
	HEAP->s[HEAP->heap_size].level = newnode->level;
	HEAP->s[HEAP->heap_size].profit = newnode->profit;
	HEAP->s[HEAP->heap_size].weight = newnode->weight;

	parent = HEAP->heap_size / 2;

	while (parent > 0)
	{
		shift_down(HEAP, parent);
		parent = parent / 2;
	}
}

basket delete(heap *HEAP)
{
	basket out;

	out = HEAP->s[1];
	HEAP->s[1] = HEAP->s[HEAP->heap_size];
	HEAP->heap_size = HEAP->heap_size - 1;
	shift_down(HEAP, 1);

	return out;
}

double bound(basket bas)
{
	int j, k;
	double total_weight;
	double result;

	if (bas.weight >= W)
		return 0;
	else
	{
		result = bas.profit;
		j = bas.level + 1;
		total_weight = bas.weight;

		while (j <= n && total_weight + w[j] <= W)
		{
			total_weight = total_weight + w[j];
			result = result + p[j];
			j++;
		}

		k = j;

		if (k <= n)
			result = result + (W - total_weight)*(p[k] / w[k]);

		return result;
	}
}

void knapsack(double *max_profit, basket *maxnode) 
{
	basket one, two;
	heap HEAP;

	HEAP.heap_size = 0;
	heap_limit = 100; 
	HEAP.s = (basket *)malloc(sizeof(basket)*heap_limit);
	
	two.level = 0;
	two.profit = 0;
	two.weight = 0;
	*max_profit = 0;

	two.boundary = bound(two);

	one.inORnot = (int *)malloc(sizeof(int)*n);
	memset(one.inORnot, 0, sizeof(int)*n);
	two.inORnot = (int *)malloc(sizeof(int)*n);
	memset(two.inORnot, 0, sizeof(int)*n);

	insert(&HEAP, &two);
	free(two.inORnot);

	while (HEAP.heap_size != 0)
	{
		two = delete(&HEAP);

		if (two.boundary > *max_profit)
		{
			one.level = two.level + 1;
			one.weight = two.weight + w[one.level];
			one.profit = two.profit + p[one.level];
			memcpy(one.inORnot, two.inORnot, sizeof(int)*n);

			if (one.weight <= W && one.profit > *max_profit)
			{
				*max_profit = one.profit;
				one.inORnot[one.level - 1] = 1;
				memcpy(maxnode->inORnot, one.inORnot, sizeof(int)*n);
			}

			one.boundary = bound(one);

			if (one.boundary > *max_profit)
			{
				one.inORnot[one.level - 1] = 1;
				insert(&HEAP, &one);
			}

			one.weight = two.weight;
			one.profit = two.profit;
			one.boundary = bound(one);

			if (one.boundary > *max_profit)
			{
				one.inORnot[one.level - 1] = 0;
				insert(&HEAP, &one);
			}
		}

		free(two.inORnot);
	}

	free(HEAP.s);
	free(one.inORnot);

}

void main()
{
	FILE *fin, *fout;
	char s[100];
	int i, j;
	int *original; 
	int tempS;
	int *sequence; 
	double tempP, tempW;
	double max_profit;
	basket maxnode;

	printf("input file name? ");
	scanf("%s", &s);
	
	fin = fopen(s, "r");
	fout = fopen("output.txt", "w");

	fscanf(fin, "%d %lf", &n, &W);
	
	w = (double *)malloc(sizeof(double)*(n + 1));
	p = (double *)malloc(sizeof(double)*(n + 1));
	original = (int *)malloc(sizeof(double)*n);
	sequence = (int *)malloc(sizeof(int)*(n + 1));

	for (i = 1; i <= n; i++) 
		sequence[i] = i;

	for (i = 0; i<n; i++)
	{
		fscanf(fin, "%lf %lf", &p[i + 1], &w[i + 1]);
	}

	for (i = 1; i < n; i++) 
	{
		for (j = n - 1; j >= i; j--)
		{
			if (p[j] / w[j] < p[j + 1] / w[j + 1])
			{
				tempP = p[j];
				tempW = w[j];
				p[j] = p[j + 1];
				w[j] = w[j + 1];
				p[j + 1] = tempP;
				w[j + 1] = tempW;
				tempS = sequence[j];
				sequence[j] = sequence[j + 1];
				sequence[j + 1] = tempS;
			}
		}
	}

	maxnode.inORnot = (int *)malloc(sizeof(int)*n);
	knapsack(&max_profit, &maxnode);

	for (i = 0; i<n; i++) 
		original[sequence[i + 1] - 1] = maxnode.inORnot[i];
	
	fprintf(fout, "%d", (int)max_profit);
	for (i = 0; i<n; i++)
	{
		fprintf(fout, "\n%d", original[i]);
	}

	free(w);
	free(p);
	free(sequence);
	free(original);
	
}