#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


int *arr;

struct data
{
  int p, r;
} ;


void merge(int p,  int q, int r)
{   
    int i, j, k;
    int n1 = q - p + 1;
    int n2 =  r - q;
    int L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = arr[p + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[q + 1 + j];
    i = 0;
    j = 0;
    k = p;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}
void * mergeSort(void * arg)
{   
    struct data *m  = (struct data*)arg;
    pthread_t t1, t2; 
    struct data m1, m2;
    if (m->p < m->r) {
        int q = m->p + (m->r - m->p) / 2;
        m1.p = m->p;
        m1.r = q;
        m2.p = q + 1;
        m2.r = m->r;
        pthread_create(&t1, NULL, &mergeSort, (void *)&m1);
        pthread_create(&t2, NULL, &mergeSort, (void *)&m2);
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        merge(m->p, q, m->r);
    }
}
void printArray(int arr[], int size) {
  for (int i = 0; i < size; i++)
    printf("%d   ", arr[i]);
    printf("\n");
}
int main()
{   
    char filename[20];
    printf("Enter your filename (.txt): ");
    scanf("%s",filename);
    strcat(filename,".txt");
    FILE *fp = fopen(filename, "r");
    int size;
    if(fp == NULL)
   {
    printf("File not found!\n");
    exit(0);
    }
    else
  { 
    
    fscanf(fp, "%d", &size);
    arr = malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
    fscanf(fp,"%d ",&arr[i]);
  }
   fclose(fp);
   
    struct data m; 
    m.p = 0;
    m.r = size - 1;
    printf("\nArray before Sorting:\n");
    printArray(arr,size);
    mergeSort((void *)&m);
    printf("\nArray after Sorting:\n");
    printArray(arr,size);
   
    return 0;
}
