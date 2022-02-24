#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

// To store parameters for solveSudoku
struct fun_params {
    int* grid;
    int row, col, N;
    int* b;
};

void PrintTable(int *grid, int n)
{
	for (int i = 0; i < n; i++)	{
		for (int j = 0; j < n; j++)
			printf("%d ", *((grid + i*n) + j));
		printf("\n");
	}
}

int isSafe(int* grid, int row, int col, int num, int n) {

	// Check for same num in the same column
	for (int x = 0; x <= n-1; x++)
		if (*((grid + x*n) + col) == num)
			return 0;
    
    // Check for same num in the same row
	for (int x = 0; x <= n-1; x++)
		if (*((grid + row*n) + x) == num)
			return 0;

    // Check for same num in smaller square
    int x = sqrt(n);
	int startRow = row - row%3;
    int startCol = col - col%3;

	for (int i = 0; i < x; i++)
		for (int j = 0; j < x; j++)
			if (*((grid + (i+startRow)*n) + (j+startCol)) == num)
				return 0;

	return 1;
}

// multi-threaded sudoku solver
void* solveSudoku(void* args)
{
    struct fun_params* params = (struct fun_params*) args;
    // int* grid = (int*)malloc(params->N*params->N*sizeof(int));
    // for(int i=0; i<params->N*params->N; i++) {
    //     *(grid+i) = *(params->grid+i);
    // }

    printf("%d %d\n", params->row, params->col);
    // PrintTable(grid, params->N);

	if (params->row == params->N-1 && params->col == params->N) {
        *(params->b) = 1;
        PrintTable(params->grid, params->N);
    }

	if (params->col == params->N) {
		params->row++;
		params->col = 0;
	}

	if (*((params->grid+params->row*params->N) + params->col) > 0) {
        params->col++;
        solveSudoku(args);
    }

    else {
        // pthread_t* threads = (pthread_t*)malloc(params->N*sizeof(pthread_t));
        pthread_t threads[params->N];
        struct fun_params param1[params->N];
        int** grid = (int**)malloc(params->N*sizeof(int*));

        int n = 0;
    	for (int num = 1; num <= params->N; num++) {
            if (isSafe(params->grid, params->row, params->col, num, params->N)) {
                
                *((params->grid+params->row*params->N) + params->col) = num;
                
                if(!*(params->b)) {
                    // struct fun_params param = { .grid=grid, .row=params->row, .col=params->col, .N=params->N, .b=params->b };

                    grid[n] = (int*)malloc(params->N*params->N*sizeof(int));
                    for(int i=0; i<params->N*params->N; i++) {
                        grid[n][i] = *(params->grid+i);
                    }

                    param1[n].grid=grid[n]; 
                    param1[n].row=params->row; 
                    param1[n].col=params->col; 
                    param1[n].N=params->N; 
                    param1[n].b=params->b;
                    
                    pthread_create(&threads[n], NULL, solveSudoku, (void*)&param1[n]);
                    n++;
                    // pthread_t thread;
                    // pthread_create(&thread, NULL, solveSudoku, (void*)&param);
                    // pthread_join(thread, NULL);
                }
            }
        
            // *((grid+params->row*params->N) + params->col) = 0;
        }

        for(int i=0; i<n; i++) {
            pthread_join(threads[i], NULL);
        }
    }

    return NULL;
}

int main()
{
    FILE *fp = fopen("ip_file.txt", "r");;
    int size;
    fscanf(fp, "%d", &size);

    int* grid = (int*)malloc(sizeof(int)*size*size);
    for (int i = 0; i < size; i++) 
        for (int j = 0; j < size; j++) {
            fscanf(fp, "%d", grid+size*i+j);
    }

    PrintTable(grid, size);
    printf("\n");

    int b = 0;
    struct fun_params params = {.grid = grid, 
                                .row = 0, 
                                .col = 0, 
                                .N = size, 
                                .b = &b };
    pthread_t thread;
    pthread_create(&thread, NULL, solveSudoku, (void*)&params);
    pthread_join(thread, NULL);

	return 0;
}
