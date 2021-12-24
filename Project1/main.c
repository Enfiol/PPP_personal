#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <time.h>

int* loadMatrix(/*FILE* inputFile,*/ int m, int n)
{
    
    double *matrix = (double*)malloc(sizeof(double)*n*m);
   
   
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
            matrix[i*m + j] = rand() % 100;
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

void reverseMatrix(double* matrix, double* submatrix, int m, int n, int rank, int nproc)
{
    double coef;
    int partCount = m / nproc + (m % nproc != 0);
    double* part = (double*)malloc(sizeof(double) * m);
    double* subpart = (double*)malloc(sizeof(double) * m);
    memset(part, 0, sizeof(double) * m);
    memset(subpart, 0, sizeof(double) * m );
    double* tmpMatrix = (double*)malloc(sizeof(double) * m * n);
    double* tmpSubmatrix = (double*)malloc(sizeof(double) * m * n);
    memset(tmpMatrix, 0, sizeof(double) * m * n);
    memset(tmpSubmatrix, 0, sizeof(double) * m * n);
    // Преобразование к единичной
    for (int c = 0; c < n; c++)
    {
        if (rank == 0)
        {
            coef = matrix[c * n + c];
        }
        MPI_Bcast(&coef, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        for(int i = partCount * rank; i < partCount * (rank+1) && i < m; i++)
        {
            part[i] = matrix[c * n + i] / coef;
            subpart[i] = submatrix[c * n + i] / coef;
        }

        MPI_Allreduce(part, &matrix[c * m], m, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(subpart, &submatrix[c * m], m, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        /*printf("S\n");
        printMatrix(stdout, matrix, m, n);
        printMatrix(stdout, submatrix, m, n);*/
        
        //-----------
        for (int i = rank; i < m; i+=nproc)
        {
            if (i == c)
            {
                for (int j = 0; j < n; j++)
                {
                    tmpMatrix[i * n + j] = matrix[i * n + j];
                    tmpSubmatrix[i * n + j] = submatrix[i * n + j];
                }
                continue;
            }
            coef = matrix[i * n + c];
            for (int j = 0; j < n; j++)
            {
                tmpMatrix[i * n + j] = matrix[i * n + j] - matrix[c * n + j] * coef;
                tmpSubmatrix[i * n + j] = submatrix[i * n + j] - submatrix[c * n + j] * coef;
            }
            MPI_Allreduce(&tmpMatrix[i*n], &matrix[i*n], m, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
            MPI_Allreduce(&tmpSubmatrix[i*n], &submatrix[i*n], m, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        }
        /*printf("I\n");
        printMatrix(stdout, tmpMatrix, m, n);
        printMatrix(stdout, tmpSubmatrix, m, n);*/
        
        /*printf("I\n");
        printMatrix(stdout, matrix, m, n);
        printMatrix(stdout, submatrix, m, n);*/
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
    m = 2000;
    n = 2000;


    //double* matrix = (double*)malloc(sizeof(double) * m * n);
    double* submatrix = (double*)malloc(sizeof(double) * m * n);
    memset(submatrix, 0, sizeof(double)*m*n);
    // Заполнение единичной матрицы
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (i == j)
            {
                submatrix[i * n + j] = 1;
            }
        }
    }

    double* matrix = loadMatrix(m, n);

    /*if (myrank == 0)
    {
        double numbers[] = { 2,5,7,6,3,4,5,-2,-3 };
        for (int i = 0; i < m * n; i++)
        {
            matrix[i] = numbers[i];
        }
    }*/

    MPI_Bcast(matrix, m * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(submatrix, m * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (myrank == 0)
    {
        //printMatrix(stdout, matrix, m, n);
    }
    int begin = clock();
    reverseMatrix(matrix, submatrix, m, n, myrank, nprocs);
    int end = clock();
    if (myrank == 0)
    {
        //printMatrix(stdout, submatrix, m, n);
        printf("Time: %d\n", (end - begin) / CLOCKS_PER_SEC);
    }
    MPI_Finalize();
    return 0;
}