#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#pragma warning(disable : 4996)

int *col;
int num;
int stop = 0;

void n_queen(int i) {

	int k;
	if (promising(i) == TRUE)
	{
		if (i == num)
		{
			stop = 1;
			return;
		}
		else
		{
			for (k = 1; k <= num; k++)
			{
				if (stop)
					break;
				col[i + 1] = k;
				n_queen(i + 1);
			}
		}
	}
}

int promising(int i) {

	int k = 1;
	int promising = TRUE;

	while (k<i && promising == TRUE)
	{
		if (col[i] == col[k] || (abs(col[i] - col[k])) == abs(i - k))
			promising = FALSE;

		k++;
	}

	return promising;
}

int estimate(int n)
{
	int i = 0;
	int j, *promising;
	int promising_count = 1;
	int temp;

	srand((long)time(NULL));

	promising = (int*)malloc(sizeof(int)*(n + 1));

	while (i < n && promising_count != 0)
	{
		i++;

		for (j = 1; j <= n; j++)
			promising[j] = 1;

		for (j = 1; j < i; j++)
		{
			promising[col[j]] = 0;
			temp = col[j] - (i - j);
			if (temp > 0)
				promising[temp] = 0;
			temp = col[j] + (i - j);
			if (temp <= n)
				promising[temp] = 0;
		}

		promising_count = 0;

		for (j = 1; j <= n; j++)
		{
			if (promising[j])
			{
				promising[promising_count] = j;
				promising_count++;
			}
		}

		if (promising_count > 0)
			col[i] = promising[rand() % promising_count];
	}

	free(promising);
	return i - 1;
}

void main() {

	FILE *fin;
	FILE *fout;
	int i, random;
	char s[100];

	printf("input file name?");
	scanf("%s", &s);

	fin = fopen(s, "r");
	fout = fopen("output.txt", "w");

	fscanf(fin, "%d", &num);

	col = (int *)malloc(sizeof(int)*(num + 1));

	while (!stop)
	{
		do {
			random = estimate(num);
		} while (random < num - 5);

		random--;

		while (random >= 0)
		{
			if (random == num - 16)
				break;

			n_queen(random);

			if (stop)
				break;
			else
				random--;
		}

		if (random < 0)
			break;
	}

	for (i = 1; i < num; i++)
		fprintf(fout, "%d\n", col[i]);

	fprintf(fout, "%d\n", col[num]);

	free(col);
	fclose(fin);
	fclose(fout);
}