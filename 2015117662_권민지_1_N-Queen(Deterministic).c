#include <stdio.h>
#include <stdlib.h>

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

void main() {

	FILE *fin;
	FILE *fout;
	int i;
	char s[100];

	printf("input file name?");
	scanf("%s", &s);

	fin = fopen(s, "r");
	fout = fopen("output.txt", "w");

	fscanf(fin, "%d", &num);

	col = (int *)malloc(sizeof(int) * (num + 1));

	n_queen(0);

	for (i = 1; i < num; i++) {

		fprintf(fout, "%d\n", col[i]);
	}

	fprintf(fout, "%d", col[i]);

	free(col);
	fclose(fin);
	fclose(fout);

}
