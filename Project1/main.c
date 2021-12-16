#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int** loadMatrix(FILE* inputFile, int m, int n)
{
    
    int **matrix = (int**)malloc(sizeof(int*)*n);
    if (matrix == NULL)
    {
        printf("Cannot allocate memory for matrix");
    }
    for (int i = 0; i < n; i++)
    {
        matrix[i] = (int*)malloc(sizeof(int) * m);
        if (matrix[i] = NULL)
        {
            printf("Cannot allocate memory for row of matrix");
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            matrix[i][j] = 0;
        }
    }

    char* s;
    s = (char*)malloc(sizeof(char) * m * 2 + 100);
    char* subs;
    for(int i = 0; i < n; i++)
    {
        int res = fgets(s, n*2-1, inputFile);

        subs = strtok(s, " ");
        for(int j = 0; j < m; j++)
        {
            *(*(matrix + i)+j) = atoi(subs);
        }
    }
    free(s);
    free(subs);
    return matrix;
}

void cleanMatrix(int **matrix, int m, int n)
{

}

void printMatrix(int **matrix, int m, int n)
{
    FILE *outputFile;
    fopen("out.txt","w");
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            printf("%d ");
        }
        printf("\n");
    }
}

void reverseMatrix(int** matrix, int m, int n)
{

}

int main()
{
    FILE* inputFile = fopen("in.txt", "r");
    int m, n;
    fscanf(inputFile, "%d %d\n", &m, &n);
    int **matrix = loadMatrix(inputFile, m, n);
    printMatrix(matrix, m, n);
    
    return 0;
}