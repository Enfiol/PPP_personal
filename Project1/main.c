#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

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


void printMatrix(FILE *outputFile, double *matrix, int m, int n)
{
    
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            fprintf(outputFile, "%f ", matrix[i * n + j]);
        }
        fprintf(outputFile, "\n");
    }
    fprintf(outputFile, "_____________\n");
    
}

void reverseMatrix(double* matrix, double* submatrix, int m, int n, int rank, int nprocs)
{   
    // Преобразование к единичной
    double* tmpMatrix = (double*)malloc(sizeof(double) * m * n);
    double* tmpSubmatrix = (double*)malloc(sizeof(double) * m * n);
    double coef;
    for (int c = rank; c < n; c += nprocs)
    {
        if (rank != 0)
        {
            for (int i = 0; i < m * n; i++)
            {
                tmpMatrix[i] = 1;
                tmpSubmatrix[i] = 1;
            }
        }
        else
        {
            for (int i = 0; i < m * n; i++)
            {
                tmpMatrix[i] = matrix[i];
                tmpSubmatrix[i] = submatrix[i];
            }
        }
        coef = matrix[c * n + c];


        if (rank != 0)
        {
            for (int i = 0; i < m; i++)
            {
                if (coef != 0)
                {
                    tmpMatrix[c * n + i] = 1 / coef;
                }
                if (coef != 0)
                {
                    tmpSubmatrix[c * n + i] = 1 / coef;
                }
            }
        }
        else
        {
            for (int i = 0; i < m; i++)
            {
                if (coef != 0)
                {
                    tmpMatrix[c * n + i] = matrix[c * n + i] / coef;
                }
                if (coef != 0)
                {
                    tmpSubmatrix[c * n + i] = submatrix[c * n + i] / coef;
                }
            }
        }

        MPI_Allreduce(tmpMatrix, matrix, m * n, MPI_DOUBLE, MPI_PROD, MPI_COMM_WORLD);
        MPI_Allreduce(tmpSubmatrix, submatrix, m * n, MPI_DOUBLE, MPI_PROD, MPI_COMM_WORLD);
    }
    printf("b");
    printf("m\n");
    printMatrix(stdout, matrix, m, n);
    printf("s\n");
    printMatrix(stdout, submatrix, m, n);
    for (int c = rank; c < n; c += nprocs)
    {
        for (int i = 0; i < m * n; i++)
        {
            tmpMatrix[i] = 0;
            tmpSubmatrix[i] = 0;
        }
       
        for (int i = 0; i < m; i++)
        {
            if (i == c) continue;
            coef = matrix[i * n + c];
            if (rank != 0)
            {
                for (int j = 0; j < n; j++)
                {
                    tmpMatrix[i * n + j] = -(matrix[c * n + j] * coef);
                    tmpSubmatrix[i * n + j] = -(submatrix[c * n + j] * coef);
                }
            }
            else
            {
                for (int j = 0; j < n; j++)
                {
                    tmpMatrix[i * n + j] = matrix[i * n + j] - (matrix[c * n + j] * coef);
                    tmpSubmatrix[i * n + j] = submatrix[i * n + j] - (submatrix[c * n + j] * coef);
                }
            }
        }
        //MPI_Barrier(MPI_COMM_WORLD);
        MPI_Allreduce(tmpMatrix, matrix, m * n, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(tmpSubmatrix, submatrix, m * n, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        printf("Iter %d\n", c);
        printf("1 m\n");
        printMatrix(stdout, matrix, m, n);
        printf("2 s\n");
        printMatrix(stdout, submatrix, m, n);
    }
}

int main(int argc, char** argv)
{
    
    MPI_Init(&argc, &argv);

    int myrank, nprocs;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Status status;
    
    int m, n;
    m = 3;
    n = 3;

    double* submatrix = (double*)malloc(sizeof(double) * m * n);
    double* matrix = (double*)malloc(sizeof(double) * m * n);
    double* initialSubmatrix;
    double* initialMatrix;
    double tmp[9] = { 2,5,7,6,3,4,5,-2,-3 };
    initialMatrix = (double*)calloc(m * n, sizeof(double));
    initialSubmatrix = (double*)calloc(m * n, sizeof(double));
    if (myrank == 0)
    {
        for(int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                initialMatrix[i*n+j] = tmp[i*n+j];
                if(i==j)
                {
                    initialSubmatrix[i + n * j] = 1;
                }
               
            }
        }
    }
    

    MPI_Allreduce(initialMatrix, matrix, m * n, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(initialSubmatrix, submatrix, m * n, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    if (myrank == 0)
    {
        printMatrix(stdout, matrix, m, n);
    }
    
    
    reverseMatrix(matrix, submatrix, m, n, myrank, nprocs);
    
    
    if (myrank == 0)
    {
        printMatrix(stdout, submatrix, m, n);
    }
    
    free(initialMatrix);
    free(initialSubmatrix);
    MPI_Finalize();
    return 0;
}