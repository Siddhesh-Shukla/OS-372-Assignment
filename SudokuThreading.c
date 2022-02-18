#include <stdio.h>
#include <pthread.h>

// struct fun_params {
//     int* grid;
//     int row, col, N;
//     int* b;
// };


// void PrintTable(int *arr, int n)
// {
// 	for (int i = 0; i < n; i++)	{
// 		for (int j = 0; j < n; j++)
// 			printf("%d ", *((arr+i*n) + j));
// 		printf("\n");
// 	}
// }

// bool isSafe(int* grid, int row, int col, int num, int n)
// {
	
// 	// Check if we find the same num in the similar row
// 	for (int x = 0; x <= n-1; x++)
// 		if (*((grid+row*n) + x) == num)
// 			return false;

// 	// Check if we find the same num in the similar column
// 	for (int x = 0; x <= n-1; x++)
// 		if (*((grid+x*n) + col) == num)
// 			return false;

// 	// Check if we find the same num in the particular 3*3 matrix,
// 	int startRow = row - row % 3, startCol = col - col % 3;

// 	for (int i = 0; i < 3; i++)
// 		for (int j = 0; j < 3; j++)
// 			if (*((grid+(i + startRow)*n) + (j+startCol)) == num)
// 				return false;

// 	return true;
// }

// void* solveSudoku(void* args)
// {
//     struct fun_params* params = (struct fun_params*) args;

// 	if (row == params->N-1 && col == params->N)
// 		params->b = 1;

// 	if (params->col == params->N) {
// 		params->row++;
// 		params->col = 0;
// 	}

// 	if (*((grid+params->row*params->N) + params->col) > 0) {
//         params->col++;
//         solveSudoku(args);
//     }
//     else {
//     	for (int num = 1; num <= params->N; num++) {
//             if (isSafe(params->grid, params->row, params->col, num, params->N)) {
                
//                 *((params->grid+params->row*params->N) + params->col) = num;
                
//                 if(! params->b)
//                     struct fun_params params = { .grid=params->grid, .row=params->row, .col=params->col, .N=params->N, .b=params->b };
//             }
        
//             *((params->grid+params->row*params->N) + params->col) = 0;
//         }
//     }
//     return NULL;
// }

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
    int b = 0;
    // struct fun_params params = { .grid=(int*)grid, .row=0, .col=0, .N=N, .b=&b };
    // pthread_t thread;
    // pthread_create(&thread, NULL, solveSudoku, (void*)&params);
    // pthread_join(thread, NULL);

    // if(!b)
	// 	printf("no solution exists \n");
    // else {
    //     PrintTable((int*)grid, N);
    // }

	return 0;
}
