#include <stdio.h>


void PrintTable(int *arr, int n)
{
	for (int i = 0; i < n; i++)	{
		for (int j = 0; j < n; j++)
			printf("%d ", *((arr+i*n) + j));
		printf("\n");
	}
}


bool isSafe(int* grid, int row, int col, int num, int n)
{
	
	// Check if we find the same num in the similar row
	for (int x = 0; x <= n-1; x++)
		if (*((grid+row*n) + x) == num)
			return false;

	// Check if we find the same num in the similar column
	for (int x = 0; x <= n-1; x++)
		if (*((grid+x*n) + col) == num)
			return false;

	// Check if we find the same num in the particular 3*3 matrix,
	int startRow = row - row % 3, startCol = col - col % 3;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (*((grid+(i + startRow)*n) + (j+startCol)) == num)
				return false;

	return true;
}


bool solveSudoku(int* grid, int row, int col, int n)
{

	if (row == n-1 && col == n)
		return true;

	if (col == n) {
		row++;
		col = 0;
	}

	if (*((grid+row*n) + col) > 0)
		return solveSudoku(grid, row, col+1, n);

	for (int num = 1; num <= n; num++) {
		if (isSafe(grid, row, col, num, n)) {
			
			*((grid+row*n) + col) = num;
		
			if (solveSudoku(grid, row, col + 1, n))
				return true;
		}
	
		*((grid+row*n) + col) = 0;
	}
	return false;
}


int main()
{
	int N = 9;
	int grid[N][N] = { { 3, 0, 6, 5, 0, 8, 4, 0, 0 },
						{ 5, 2, 0, 0, 0, 0, 0, 0, 0 },
						{ 0, 8, 7, 0, 0, 0, 0, 3, 1 },
						{ 0, 0, 3, 0, 1, 0, 0, 8, 0 },
						{ 9, 0, 0, 8, 6, 3, 0, 0, 5 },
						{ 0, 5, 0, 0, 9, 0, 6, 0, 0 },
						{ 1, 3, 0, 0, 0, 0, 2, 5, 0 },
						{ 0, 0, 0, 0, 0, 0, 0, 7, 4 },
						{ 0, 0, 5, 2, 0, 6, 3, 0, 0 } };

	if (solveSudoku((int*)grid, 0, 0, N))
		PrintTable((int*)grid, N);
	else
		printf("no solution exists \n");

	return 0;
}
