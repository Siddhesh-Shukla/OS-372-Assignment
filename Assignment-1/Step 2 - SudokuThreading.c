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

// To print sudoku
void PrintTable(int *grid, int n)
{
	for (int i = 0; i < n; i++)	{
		for (int j = 0; j < n; j++)
			printf("%d\t", *((grid + i*n) + j));
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
    int val = 9;

    // printf("%d %d", params->row, params->col);

	if (params->row >= params->N-1 && params->col >= params->N) {
        *(params->b) = 1;
        PrintTable(params->grid, params->N);
        return NULL;
    }

	if (params->col >= params->N) {
		params->row++;
		params->col = 0;
	}

	if (*((params->grid+params->row*params->N) + params->col) > 0) {
        params->col++;
        solveSudoku(args);
    }

    else {
        if(params->N > val) {
            pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t)*params->N);
            struct fun_params* next_param = (struct fun_params*)malloc(sizeof(struct fun_params)*params->N);
            int** grid = (int**)malloc(params->N*sizeof(int*));

            int n = 0;
            for (int num = 1; num <= params->N; num++) {

                // printf("%d %d ", params->row, params->col);

                if (isSafe(params->grid, params->row, params->col, num, params->N)) {
                    
                    *((params->grid+params->row*params->N) + params->col) = num;
                    
                    if(!*(params->b)) {

                        grid[n] = (int*)malloc(params->N*params->N*sizeof(int));
                        for(int i=0; i<params->N*params->N; i++) {
                            grid[n][i] = *(params->grid+i);
                        }
                        
                        next_param[n].grid=grid[n]; 
                        next_param[n].row=params->row; 
                        next_param[n].col=params->col; 
                        next_param[n].N=params->N; 
                        next_param[n].b=params->b;
                        
                        pthread_create(&threads[n], NULL, solveSudoku, (void*)&next_param[n]);
                        n++;
                    }
                }
            }

            for(int i=0; i<n; i++) {
                pthread_join(threads[i], NULL);
            }

            for(int i=0; i<n; i++) {
                free(grid[i]);
            }
            free(grid);
            free(threads);
            free(next_param);
        }
        else {
    	    for (int num = 1; num <= params->N; num++) {
                if (isSafe(params->grid, params->row, params->col, num, params->N)) {
                
                    *((params->grid+params->row*params->N) + params->col) = num;
                
                    if(!*(params->b)) {
                        struct fun_params param = { .grid=params->grid, .row=params->row, .col=params->col, .N=params->N, .b=params->b };
                        pthread_t thread;
                        pthread_create(&thread, NULL, solveSudoku, (void*)&param);
                        pthread_join(thread, NULL);
                    }
                }
        
            *((params->grid+params->row*params->N) + params->col) = 0;
            } 
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int size = atoi(argv[1]);
    FILE *fp = fopen(argv[2], "r");

    int* grid = (int*)malloc(sizeof(int)*size*size);
    for (int i = 0; i < size; i++) 
        for (int j = 0; j < size; j++) {
            fscanf(fp, "%d", grid+size*i+j);
    }

    // PrintTable(grid, size);
    // printf("\n");

    int b = 0;
    struct fun_params* params = (struct fun_params*)malloc(sizeof(struct fun_params));
    params->grid = grid;
    params->row = 0; 
    params->col = 0; 
    params->N = size; 
    params->b = &b;
    pthread_t thread;
    pthread_create(&thread, NULL, solveSudoku, (void*)params);
    pthread_join(thread, NULL);

	return 0;
}
