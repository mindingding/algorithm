#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

int n;
int heap_limit;
int **Adj;

typedef struct 
{
	int level;
	int *path;
	int boundary;
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


void insert(heap *HEAP, basket *new)
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

	HEAP->s[HEAP->heap_size].path = (int*)malloc(sizeof(int)*(n+1));
	memcpy(HEAP->s[HEAP->heap_size].path, new->path, sizeof(int)*(n+1));
	HEAP->s[HEAP->heap_size].boundary = new->boundary;
	HEAP->s[HEAP->heap_size].level = new->level;

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

int bound(basket *bas)
{
	int i, j, k; 
	int min; 
	int result; 
	int flag;

	result = 0;
	if (bas->level == 0)
	{
		for (i = 0; i<n; i++)
		{
			min = 9999;
			for (j = 0; j<n; j++)
			{
				if (Adj[i][j] < min && Adj[i][j] > 0)
					min = Adj[i][j];
			}
			result += min;
		}
		return result;
	}
	else
	{
		for (i = 0; i<n; i++)
		{
			flag = 0;
			for (j = 0; j<bas->level; j++)
			{
				if (bas->path[j] == i + 1)
				{
					flag = 1;
					result += Adj[i][bas->path[j + 1] - 1];
					break;
				}
			}
			if (flag)
				continue;
			if (flag == 0 && bas->path[bas->level] == i + 1)
			{
				min = 9999;
				for (j = 1; j<n; j++)
				{
					flag = 0;
					for (k = 1; k <= bas->level; k++)
					{
						if ((j + 1) == bas->path[k])
							flag = 1;
					}
					if (!flag && Adj[i][j] < min)
						min = Adj[i][j];
				}
				result += min;
			}
			else
			{
				min = 9999;
				for (j = 0; j<n; j++)
				{
					flag = 0;
					for (k = 1; k <= bas->level; k++)
					{
						if ((j + 1) == bas->path[k])
							flag = 1;
					}
					if (!flag&& Adj[i][j] < min && Adj[i][j] > 0)
						min = Adj[i][j];
				}
				result += min;
			}
		}
	}

	return result;
}

void travel(int *optimal_path)
{
	heap HEAP;
	basket u, v;
	int min, tem_length; 
	int i, j, k; 
	int flag;
	
	HEAP.heap_size = 0;
	heap_limit = 100; 
	HEAP.s = (basket *)malloc(sizeof(basket)*heap_limit);
	
	u.path = (int *)malloc(sizeof(int)*(n + 1));
	v.level = 0;
	v.path = (int *)malloc(sizeof(int)*(n + 1));
	v.path[0] = 1;
	v.boundary = bound(&v);

	min = 9999;
	insert(&HEAP, &v);
	free(v.path);

	while (HEAP.heap_size != 0)
	{
		v = delete(&HEAP);
		if (v.boundary < min)
		{
			u.level = v.level + 1;
			for (i = 2; i <= n; i++)
			{
				flag = 0;
				for (j = 1; j <= v.level; j++)
				{
					if (v.path[j] == i)
					{
						flag = 1;
						break;
					}
				}
				if (flag)
					continue;
				else
				{
					memcpy(u.path, v.path, sizeof(int)*(n + 1));
					u.path[u.level] = i;
				}
				if (u.level == n - 2)
				{
					for (j = 2; j <= n; j++)
					{
						flag = 0;
						for (k = 1; k <= u.level; k++)
						{
							if (j == u.path[k])
								flag = 1;
						}
						if (!flag)
						{
							u.path[u.level + 1] = j;
							break;
						}
					}
					u.path[u.level + 2] = 1;
					tem_length = bound(&u);
					if (tem_length < min)
					{
						min = tem_length;
						memcpy(optimal_path, u.path, sizeof(int)*(n + 1));
					}
				}
				else
				{
					u.boundary = bound(&u);
					if (u.boundary < min)
						insert(&HEAP, &u);
				}
			}
		}
		free(v.path);
	}
	free(HEAP.s);
	free(u.path);
}

void main()
{
	FILE *fin, *fout;
	char s[100];
	int *optimal_path;
	int i;
	int j;

	printf("input file name?");
	scanf("%s", &s);

	fin = fopen(s, "r");
	fout = fopen("output.txt", "w");

	fscanf(fin, "%d", &n);

	optimal_path = (int*)malloc(sizeof(int)*(n + 1));
	Adj = (int**)malloc(sizeof(int*)*n);

	for (i = 0; i<n; i++)
		Adj[i] = (int *)malloc(sizeof(int)*n);

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			fscanf(fin, "%d", &Adj[i][j]);
		}
	}

	travel(optimal_path);

	for (i = 0; i<n; i++)
		fprintf(fout, "%d\n", optimal_path[i]);
	fprintf(fout, "%d", optimal_path[n]);

	free(optimal_path);

	for (i = 0; i<n; i++)
	{
		free(Adj[i]);
	}

	free(Adj);
}
