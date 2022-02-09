#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

int **arr1, **arr2, **mult1, **mult2 ;
int multRows, multCols, commonIndex;

struct matrix
{
  int row, column;
} ;
 struct matrix* m; 

void * multiply_byElement(void * arg)
{
   struct matrix *m  = (struct matrix*)arg;
   for(int i = 0; i < commonIndex ; i++)
  { 
      mult1[m->row][m->column] += arr1[m->row][i] * arr2[i][m->column]; 
  }
}

void create_threads_byElement()
{ 
  int r = 0;
  m = malloc(multRows * multCols * sizeof(struct matrix));
  pthread_t t[multRows * multCols];
  for(int i = 0; i < multRows; i++)
  {
     for(int j = 0; j < multCols; j++)
     { 
        m[r].row = i;
        m[r].column = j; 
        pthread_create(&t[r], NULL, &multiply_byElement, (void *)&m[r]);
        r++;
     }
  }
  for(int k = 0; k < multRows * multCols; k++)
  {
    pthread_join(t[k], NULL);
  }
}
void * multiply_byRow(void * arg)
{
  int  * rowptr   = (int *)arg;
  int row = * rowptr;
   for(int i = 0; i < multCols ; i++)
  {  
    for(int j = 0; j < commonIndex; j++) 
      mult2[row][i] += arr1[row][j] * arr2[j][i]; 
  }
}

void create_threads_byRow()
{ 
  pthread_t t[multRows];
  int *rowIndex = malloc(multRows * sizeof(int));
  for(int i = 0; i < multRows; i++)
  {
     rowIndex[i] = i;
     pthread_create(&t[i], NULL, &multiply_byRow, (void *)&rowIndex[i]);
  }
  for(int k = 0; k < multRows; k++)
  {
    pthread_join(t[k], NULL);
  }
}

void print2DArray(int **arr, int rows, int cols)
{ 
   for(int i = 0; i < rows; i++)
    {
      for(int j = 0; j < cols; j++)
      { 
         printf("%d\t", arr[i][j]); 
      }
      printf("\n");
    }
    printf("\n");
}


int main()
{   
    char filename[20];
    printf("Enter your filename (.txt): ");
    scanf("%s",filename);
    strcat(filename,".txt");
    FILE *fp = fopen(filename, "r");
    int nrows1, ncolumns1, nrows2, ncolumns2;
    if(fp == NULL)
   {
    printf("File not found!\n");
    exit(0);
    }
    else
  { 
    fscanf(fp, "%d ", &nrows1);
    fscanf(fp, "%d ", &ncolumns1);
    
    arr1 = (int **) malloc(sizeof(int*) * nrows1); 
    for(int i = 0; i < nrows1; i++)
       arr1[i] = (int *)malloc(sizeof(int*) * ncolumns1);  
       
           
    for(int i = 0; i < nrows1; i++)
      {
      for(int j = 0; j < ncolumns1; j++)
         fscanf(fp, "%d ",&arr1[i][j]);   
      }
      
    fscanf(fp, "%d ", &nrows2);
    fscanf(fp, "%d ", &ncolumns2);
    
    arr2 = (int **) malloc(sizeof(int*) * nrows2);  
    for(int i = 0; i < nrows2; i++)
      arr2[i] = (int *)malloc(sizeof(int*) * ncolumns2);  
      
    for(int i = 0; i < nrows2; i++)
     {
      for(int j = 0; j < ncolumns2; j++)
         fscanf(fp, "%d ",&arr2[i][j]); 
     }
  }
    if(ncolumns1 != nrows2)
    {
      printf("Can't Multiply! No. of columns in the first matrix is not equal to No. of rows in the second matrix.\n");
      exit(0);
    }
    else
    {
    multRows = nrows1;
    multCols = ncolumns2;
    commonIndex = nrows2;  // = ncolumns1
    
    mult1 = (int **) malloc(sizeof(int*) * multRows);  
    for(int i = 0; i < multRows; i++) 
      mult1[i] = (int *)malloc(sizeof(int*) * multCols);  
    
    mult2 = (int **) malloc(sizeof(int*) * multRows); 
    for(int i = 0; i < multRows; i++)
      mult2[i] = (int *)malloc(sizeof(int*) * multCols);  
    
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    create_threads_byElement();
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nMultiplication By Element:\n");
    print2DArray(mult1, multRows, multCols);
    printf("Time taken in Multiplication By Element = %f s\n", cpu_time_used );
    printf("\n=========================================================\n");
    start = clock();
    create_threads_byRow();
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Multiplication By Row:\n");
    print2DArray(mult2, multRows, multCols);
    printf("Time taken in Multiplication By Row = %f s\n", cpu_time_used );
    
   }  
  
   fclose(fp);

   
    return 0;
}
