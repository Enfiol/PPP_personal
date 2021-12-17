#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <mpi.h>

int** loadMatrix(/*FILE* inputFile,*/ int m, int n)
{
    
    double **matrix = (int**)malloc(sizeof(int*)*n);
    if (matrix == NULL)
    {
        printf("Cannot allocate memory for matrix\n");
        exit(1);
    }
    for (int i = 0; i < n; i++)
    {
        matrix[i] = (int*)malloc(sizeof(double) * m);
        if (matrix[i] == NULL)
        {
            printf("Cannot allocate memory for row of matrix\n");
            exit(1);
        }
    }
    // Матрица из файла
    /*char* s;
    s = (char*)malloc(sizeof(char) * 100);
    char c;

    for(int i = 0; i < n; i++)
    {
        
        for(int j = 0; j < m; j++)
        {
            for (int k = 0; k < 100 && s[k]!='\0'; k++)
            {
                s[k] = 0;
            }
            for (int k = 0; (c = fgetc(inputFile)) != EOF && c != '\n' && c != ' '; k++)
            {
                s[k] = c;
            }
            matrix[i][j] = atoi(s);
        }
    }
    
   
    if (s)
    {
        free(s);
    }*/

    // Генерация матрицы
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = rand() % 100;
        }
    }
    
    
    return matrix;
}


void printMatrix(FILE *outputFile, double **matrix, int m, int n)
{
    
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            fprintf(outputFile, "%f ", matrix[i][j]);
        }
        fprintf(outputFile, "\n");
    }
    fprintf(outputFile, "_____________\n");
    
}

double ** reverseMatrix(double** matrix, int m, int n)
{
    // Выделение матрицы
    double** reverseMatrix = (int**)malloc(sizeof(int*) * n);
    if (reverseMatrix == NULL)
    {
        printf("Cannot allocate memory for matrix\n");
        exit(1);
    }
    for (int i = 0; i < n; i++)
    {
        reverseMatrix[i] = (int*)malloc(sizeof(double) * m);
        if (reverseMatrix[i] == NULL)
        {
            printf("Cannot allocate memory for row of matrix\n");
            exit(1);
        }
    }
    // Заполнение единичной матрицы
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (i == j)
            {
                reverseMatrix[i][j] = 1;
            }
            else
            {
                reverseMatrix[i][j] = 0;
            }
        }
    }
    // Преобразование к единичной
    for (int c = 0; c < n; c++)
    {
        double coef = matrix[c][c];
        for(int i = 0; i < m; i++)
        {
            matrix[c][i] = matrix[c][i] / coef;
            reverseMatrix[c][i] = reverseMatrix[c][i] / coef;
        }
        for (int i = 0; i < m; i++)
        {
            if (i == c) continue;
            coef = matrix[i][c];
            for (int j = 0; j < n; j++)
            {
                matrix[i][j] -= matrix[c][j] * coef;
                reverseMatrix[i][j] -= reverseMatrix[c][j] * coef;
            }
        }
       
    }
    free(matrix);
    matrix = reverseMatrix;
    return matrix;
}




int main(int argc, char** argv)
{
    /*
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    int myrank, nprocs;
    MPI_Status status;
    */
    FILE* outputFile = fopen("out.txt", "w");
    //FILE* inputFile = fopen("in.txt", "r");
    int m, n;
    m = 3000;
    n = 3000;
    //fscanf(inputFile, "%d %d\n", &m, &n);
    double **matrix = loadMatrix(/*inputFile,*/ m, n);
    printMatrix(outputFile, matrix, m, n);
    matrix = reverseMatrix(matrix, m, n);
    printMatrix(outputFile, matrix, m, n);
    free(matrix);
    fclose(outputFile);
    //MPI_Finalize();
    return 0;
}